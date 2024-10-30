Nova versão, servidor.py

Servidor http para a integração, mas com python.
- criar venv
- pip3 install pacotes

ToDo: 
- melhorar este documento.
- decodificar o recebido da yr
- fazer a devolução do dado (ver arquivo app.js)







Arquivos para integração com HTTP.

Servidor para receber um uplink e automaticamente fazer um downlink.

```
mkdir HTTPServer
cd HTTPServer
npm init -y
npm install mqtt express body-parser @grpc/grpc-js google-protobuf @chirpstack/chirpstack-api --save
```
