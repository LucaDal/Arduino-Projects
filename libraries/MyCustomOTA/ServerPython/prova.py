import json
f = open('./device.json')
API_TOKEN_LIST = json.load(f)
f.close()
print("hekllo")
print(API_TOKEN_LIST['THIS_IS_MY_OWN_API_KEY'][0]['FilePath'])