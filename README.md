# ESP32 Tally Light

1. Upload software to your ESP32
1. Configure WiFi settings using [https://github.com/sampie777/ESP32TallyLightConfigurer](https://github.com/sampie777/ESP32TallyLightConfigurer)
1. Toggle Tally Light on/off using API urls

## API

Server is running on default webserver port `80`. Calls are TCP requests.

| Endpoint | Description |
| --- | --- |
| `/` | Responds with HTML page with links to on/off |
| `/on` | Turn light on. Responds with 'on' |
| `/off` | Turn light off. Responds with 'off' |

## IP address?

You can get the IP address by pressing the Ident-button. Assuming you know your network a bit, only the last byte/section of the IP (Ipv4) address will be displayed. Displaying is done by flashing the byte code of the address. Long flash = 1; Short flash = 0. 

Assuming the IP is 192.168.178.10, the Tally Light will flash (10 (dec) => `00001010` (bin)):

| Flash | Translates to bit |
| --- | --- |
| short | 0 |
| short | 0 |
| short | 0 |
| short | 0 |
| long | 1 |
| short | 0 |
| long | 1 |
| short | 0 |
