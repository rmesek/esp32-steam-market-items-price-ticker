# ESP32 steam market items price ticker
## Prerequisites
* [LILYGO® TTGO T-Display ESP32 WiFi and Bluetooth Module Development Board For Arduino 1.14 Inch LCD](http://www.lilygo.cn/prod_view.aspx?TypeId=50044&Id=1126&FId=t3:50044:3)
* [Arduino IDE](https://www.arduino.cc/en/software)

## Arduino IDE Setup
1. In arduino IDE, go to `File > Preferences`.

![1](https://user-images.githubusercontent.com/77611276/126712844-6221095a-f879-4882-bbed-f5e7c5ae92f6.png)

2. Click “Additional Board Manager URLs” button.

![2](https://user-images.githubusercontent.com/77611276/126713276-899d88e8-da88-4eda-a32e-3cf5b70e08e3.png)

3. Enter these urls into the box.
```
https://dl.espressif.com/dl/package_esp32_index.json
https://github.com/bblanchon/ArduinoJson.git
https://github.com/taranais/NTPClient
```

![3](https://user-images.githubusercontent.com/77611276/126713340-be19b27d-30f7-4f73-8c3f-a5261c4c0cbd.png)

4. Go to `Tools > Board > Boards Manager`.

![4](https://user-images.githubusercontent.com/77611276/126713503-3e5a86ef-8f98-40d3-a3b1-9e809b4b07a2.png)

5. Search for `esp32` and click "Install".

![5](https://user-images.githubusercontent.com/77611276/126713553-a32ae4dc-11e5-488c-884c-9ef2636e26d3.png)

6. Now go to `Sketch > Include Library > Manage Libraries...`.

![6](https://user-images.githubusercontent.com/77611276/126713762-0917f6a8-2d05-472e-a357-37f3764e0c2f.png)

7. Search for `tft_espi` and install it.

![7](https://user-images.githubusercontent.com/77611276/126713894-54ef6498-56eb-4e66-bbff-ed26d9ea4699.png)

8. Search for `ntp` and install it too.

![8](https://user-images.githubusercontent.com/77611276/126713951-73b3a745-ab81-47d2-8d06-279744d05c4d.png)

9. Select `Tools > Board > ESP32 Arduino > DOIT ESP32 DEVKIT V1` and check if "Upload Speed", "Flash Frequency" and "Port" are the same as in picture below.

![9](https://user-images.githubusercontent.com/77611276/126714157-7cbe1ba1-0680-4922-a876-00725b7ef79d.png)

10. Last thing to do is find `C:\Users\[User]\Documents\Arduino\libraries\TFT_eSPI\User_Setup_Select.h` and do two things there.
    
    1. Comment `line 22`

    ![10](https://user-images.githubusercontent.com/77611276/126714742-c4f33ddb-3d84-45f2-a1b2-6e38b52f10e6.png)

    2. Uncomment `line 53`

    ![11](https://user-images.githubusercontent.com/77611276/126714813-c04cb98d-59d0-4465-ab6e-9dbd651cf40c.png)
