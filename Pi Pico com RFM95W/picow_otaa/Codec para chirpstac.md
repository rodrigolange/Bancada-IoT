no Chirpstack:

Device Profiles -> Coded -> JavaScript Function

```
function decodeUplink(input)
{
  return{
    data: Decode(input.fPort, input.bytes, input.variables)
  };
}

function Decode(fPort, bytes, variables) {
	if(bytes.length > 4){
      let temperature = ((bytes[0] << 8 | bytes[1]))/100;
      var result = "";
      for(var i = 2; i < bytes.length; ++i){
        result+= (String.fromCharCode(bytes[i]));
      }
      var decodedStr = (result).toString('base64');
      return {
        "Temperatura": temperature,
        "Dados": decodedStr
      };
    }else{
      return {
        "DadosRaw": bytes
      };
    }
}
```
