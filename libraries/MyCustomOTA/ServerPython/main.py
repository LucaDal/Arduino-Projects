import hashlib
import os
import sys
import json
from flask import Flask, send_file, json, request

f = open(os.path.join(sys.path[0], "device.json"))
API_TOKEN_LIST = json.load(f)
f.close()
app = Flask(__name__)

# ================================================

@app.route('/api/post/update/<api_key>', methods=['GET','POST'])
def api_update_prova(api_key):
    print(request.headers['X-Esp8266-Version'])
    if api_key in API_TOKEN_LIST:
        try:
            return send_file(API_TOKEN_LIST[api_key][0]['FilePath'])
        except Exception as e:
                return str(e)
    

def version(api_key):
    with open(API_TOKEN_LIST[api_key][0]['FilePath'], "rb") as file_to_check:
        data = file_to_check.read()
        md5_returned = hashlib.md5(data).hexdigest()
        #print('md5_checksum is', md5_returned)
    value = {
        "version": API_TOKEN_LIST[api_key][0]['version'],
        "md5Checksum": md5_returned
    }
    return json.dumps(value)


@app.route('/api/get/version/<api_key>')
def api_version(api_key):
    if api_key in API_TOKEN_LIST:
        return version(api_key)


@app.route('/')
def hello_word():
    return "<p>Hello World!</p>"


@app.route('/favicon.ico')
def favicon():
    return send_file('./static/favicon.ico', mimetype='image/vnd.microsoft.icon')


if __name__ == '__main__':
    app.run(host='192.168.1.4', port=9001, debug=True)