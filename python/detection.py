import cv2
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision
from collections import deque
import numpy as np
import paho.mqtt.client as mqtt
import time
from ultralytics import YOLO

# ================= MQTT =================
MQTT_IP = "10.103.150.183"

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.connect(MQTT_IP, 1883, 60)
client.loop_start()

last_send = 0

def send_command(cmd):
    global last_send
    if time.time() - last_send > 0.2:
        client.publish("robot/cmd", cmd + "\n")
        last_send = time.time()

# ================= CAMERAS =================
gesture_cam = cv2.VideoCapture(0)
tcp_stream = "tcp://10.103.150.183:8494"
cam = cv2.VideoCapture(tcp_stream, cv2.CAP_FFMPEG)

# ================= MEDIAPIPE =================
base_options = python.BaseOptions(model_asset_path='hand_landmarker.task')
options = vision.HandLandmarkerOptions(
    base_options=base_options,
    num_hands=1
)
detector = vision.HandLandmarker.create_from_options(options)

# ================= YOLO =================
fire_model = YOLO("fire_nano.pt")
human_model = YOLO("yolov8n.pt")

frame_count = 0

# ================= BUFFER =================
gesture_buffer = deque(maxlen=5)
last_command = "S"

# ================= HELPERS =================
def is_finger_folded(lm, tip, pip):
    return lm[tip][1] > lm[pip][1]

def is_finger_open(lm, tip, pip):
    return lm[tip][1] < lm[pip][1]

def distance(a, b):
    return ((a[0]-b[0])**2 + (a[1]-b[1])**2)**0.5

# ================= GESTURE LOGIC =================
def classify_gesture(lm):

    thumb_tip = lm[4]
    index_tip = lm[8]
    index_mcp = lm[5]

    index_folded = is_finger_folded(lm, 8, 6)
    middle_folded = is_finger_folded(lm, 12, 10)
    ring_folded = is_finger_folded(lm, 16, 14)
    pinky_folded = is_finger_folded(lm, 20, 18)

    index_open = is_finger_open(lm, 8, 6)
    middle_open = is_finger_open(lm, 12, 10)
    ring_open = is_finger_open(lm, 16, 14)
    pinky_open = is_finger_open(lm, 20, 18)

    # RIGHT
    if index_folded and middle_folded and ring_folded and not pinky_folded:
        return "RIGHT", "R"

    # LEFT
    if index_folded and middle_folded and ring_folded and pinky_folded:
        if thumb_tip[0] > index_mcp[0] + 0.03:
            return "LEFT", "L"

    # STOP
    if index_folded and middle_folded and ring_folded and pinky_folded:
        if abs(thumb_tip[0] - index_mcp[0]) < 0.03:
            return "STOP", "S"

    # FORWARD
    if index_open and middle_open and ring_open and pinky_open:
        return "FORWARD", "F"

    # BACKWARD
    if index_open and middle_open and ring_open and not pinky_open:
        return "BACKWARD", "B"

    # PUMP ON
    if index_open and middle_open and ring_folded:
        return "PUMP ON", "P"

    # PUMP OFF
    if distance(thumb_tip, index_tip) < 0.04:
        return "PUMP OFF", "p"

    return "NONE", "S"

# ================= MAIN LOOP =================
while True:

    # ============ GESTURE CAMERA ============
    ret1, gframe = gesture_cam.read()
    if not ret1:
        continue

    rgb = cv2.cvtColor(gframe, cv2.COLOR_BGR2RGB)
    mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb)
    result = detector.detect(mp_image)

    gesture = "NONE"
    command = "S"

    BOX_X1, BOX_Y1 = 200, 100
    BOX_X2, BOX_Y2 = 440, 380

    if result.hand_landmarks:

        if result.handedness[0][0].category_name == "Right":

            lm_raw = result.hand_landmarks[0]
            h, w, _ = gframe.shape

            pixel_lm = []

            for p in lm_raw:
                x = int(p.x * w)
                y = int(p.y * h)
                pixel_lm.append((p.x, p.y))
                cv2.circle(gframe, (x, y), 5, (0, 255, 0), -1)

            wrist_x = int(lm_raw[0].x * w)
            wrist_y = int(lm_raw[0].y * h)

            hand_in_box = BOX_X1 < wrist_x < BOX_X2 and BOX_Y1 < wrist_y < BOX_Y2

            cv2.rectangle(gframe, (BOX_X1, BOX_Y1),
                          (BOX_X2, BOX_Y2),
                          (0,255,0) if hand_in_box else (0,0,255), 3)

            if hand_in_box:
                gesture, command = classify_gesture(pixel_lm)
                gesture_buffer.append(command)

                if gesture_buffer.count(command) >= 3:

                    # 🔥 IMPROVEMENT: continuous F & B
                    if command in ["F", "B"]:
                        send_command(command)
                        last_command = command

                    # others unchanged (single trigger)
                    else:
                        if command != last_command:
                            send_command(command)
                            last_command = command

            cv2.putText(gframe, gesture, (10, 40),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,255), 2)

    # ============ ROBOT CAMERA ============
    ret2, rframe = cam.read()
    if not ret2:
        rframe = np.zeros((480,640,3), dtype=np.uint8)

    frame_count += 1

    if frame_count % 3 == 0:

        # FIRE
        fire_results = fire_model(rframe, imgsz=320, verbose=False)
        for r in fire_results:
            for box in r.boxes:
                x1,y1,x2,y2 = box.xyxy[0]
                cv2.rectangle(rframe,(int(x1),int(y1)),(int(x2),int(y2)),(0,0,255),2)

        # HUMAN
        human_results = human_model(rframe, imgsz=320, verbose=False)
        for r in human_results:
            for box in r.boxes:
                if int(box.cls[0]) == 0:
                    x1,y1,x2,y2 = box.xyxy[0]
                    cv2.rectangle(rframe,(int(x1),int(y1)),(int(x2),int(y2)),(0,255,0),2)

    # ============ COMBINE ============
    gframe = cv2.resize(gframe,(640,480))
    rframe = cv2.resize(rframe,(640,480))

    combined = np.hstack((gframe, rframe))

    cv2.imshow("ROBOT SYSTEM", combined)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

gesture_cam.release()
cam.release()
cv2.destroyAllWindows()