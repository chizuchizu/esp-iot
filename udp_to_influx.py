import os
import socket
from influxdb_client import InfluxDBClient, Point

URL = "http://localhost:8086"
TOKEN = os.getenv("INFLUXDB_TOKEN")
ORG = os.getenv("INFLUXDB_ORG", "nitkc")
BUCKET = os.getenv("INFLUXDB_BUCKET", "esp-iot")

UDP_IP = "0.0.0.0"
UDP_PORT = 12345

# Create UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

client = InfluxDBClient(url=URL, token=TOKEN, org=ORG)
write_api = client.write_api()

print("Waiting for UDP data...")

while True:
    data, addr = sock.recvfrom(1024)
    print(f"Received data: {data} from {addr}")

    # Example: 24.95,35.79
    raw = data.decode("utf-8")
    temp, humid = raw.split(",")
    print(temp, humid)

    point = Point("sensor_data").field("Temperature", float(temp))
    write_api.write(bucket=BUCKET, record=point)

    point = Point("sensor_data").field("Humidity", float(humid))
    write_api.write(bucket=BUCKET, record=point)
