import hashlib
import os
from flask import Flask, send_file, json, request

app = Flask(__name__)

# ================================================
target_file = "Simple_OTA_Application.ino.bin"
company_name = 'LucaLab'
build_num = "0.5"
build_date = '02-04-2023'
API_KEY = 'THIS_IS_MY_OWN_API_KEY'
# ================================================
path = os.path.join("C:/Users/Luca/Documents/Arduino/libraries/CustomOTA/Simple_OTA_Application/build/esp8266.esp8266.generic", target_file )

@app.route('/api/post/update&api_key=THIS_IS_MY_OWN_API_KEY&target_path=Simple_OTA_Application.ino.bin', methods=['GET','POST'])
def api_update_prova():
    print(request.headers)
    try:
        return send_file(os.path.join("C:/Users/Luca/Documents/Arduino/libraries/CustomOTA/Simple_OTA_Application/build/esp8266.esp8266.generic", "Simple_OTA_Application.ino.bin"))
    except Exception as e:
            return str(e)
    

@app.route('/api/post/update', methods=['POST'])
def api_update():
    if request.method == 'POST':
        key = request.form['api_key']
        target_path = request.form['target_path']
        if API_KEY == key and len(target_path) > 1:
            try:
                return send_file(os.path.join("C:/Users/Luca/Documents/Arduino/libraries/CustomOTA/Simple_OTA_Application/build/esp8266.esp8266.generic", target_path))
            except Exception as e:
                return str(e)


def version():
    size = os.path.getsize(path)
    print('Size of file is', size, 'bytes')

    with open(path, "rb") as file_to_check:
        data = file_to_check.read()
        md5_returned = hashlib.md5(data).hexdigest()
        print('md5_checksum is', md5_returned)

    value = {
        "companyName": company_name,
        "buildNum": build_num,
        "buildDate": build_date,
        "serverFilePath": target_file,
        "fileSize": size,
        "md5Checksum": md5_returned
    }
    return json.dumps(value)


@app.route('/api/get/version')
def api_version():
    return version()


@app.route('/')
def hello_word():
    return "<p>Hello World!</p>"


@app.route('/favicon.ico')
def favicon():
    return send_file('./static/favicon.ico', mimetype='image/vnd.microsoft.icon')


if __name__ == '__main__':
    app.run(host='192.168.1.12', port=9001, debug=True)