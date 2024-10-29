from ultralytics import YOLO

# Load the YOLO11 model
model = YOLO("yolo_classes/yolo11x.pt")

# Export the model to ONNX format
model.export(format="onnx")  # creates 'yolo11n.onnx'

# Load the exported ONNX model
onnx_model = YOLO("yolo_classes/yolo11x.onnx", task="detect")

# Run inference
results = onnx_model("https://ultralytics.com/images/bus.jpg")