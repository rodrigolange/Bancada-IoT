# https://www.chirpstack.io/docs/chirpstack/api/python-examples.html
import grpc
import struct
from chirpstack_api import api
from google.protobuf.json_format import ParseDict

class ChirpStackClient:
    def __init__(self, api_server, token):

        server = "10.0.0.30:8080"
        api_token = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdWQiOiJjaGlycHN0YWNrIiwiaXNzIjoiY2hpcnBzdGFjayIsInN1YiI6Ijk1ZTQ3OGUwLWRkOTEtNDU1Zi1iMjIzLWM5ZjhiNTMwZTllMCIsInR5cCI6ImtleSJ9.bXXZ28gDd1S_4NbW-V4Ucb1ENewO6rfD6QANHrTPKXQ"
        channel = grpc.insecure_channel(server)
        self.client = api.DeviceServiceStub(channel)
        self.auth_token = [("authorization", "Bearer %s" % api_token)]


    def send_downlink(self, device_eui, payload, f_port=1, confirmed=False):

        req = api.EnqueueDeviceQueueItemRequest()
        req.queue_item.confirmed = confirmed
        #print(payload)
        req.queue_item.data = struct.pack('d', payload)
        #print(req.queue_item.data)
        req.queue_item.dev_eui = device_eui
        req.queue_item.f_port = f_port

        try:
            resp = self.client.Enqueue(req, metadata=self.auth_token)
            print("Downlink sent successfully to %s. ID = %s" % (device_eui, resp.id))
        except grpc.RpcError as e:
            print("Error sending downlink:", e.details())
            print("Status code:", e.code())
