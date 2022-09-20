#pragma once
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <FS.h>

/*
//функция обработки входящих сообщений
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
	if (type == WStype_CONNECTED)
	{
		IPAddress ip = webSocket.remoteIP(num);
		String message = String("Connected");
		webSocket.broadcastTXT(message);   // отправляем последнее значение всем клиентам при подключении    
	}
		
	if (type == WStype_TEXT)
	{
		String message;
		for (int x = 0; x < length; x++)
		{
			message += (char) payload[x];
		}

		if (message == "reset")
		{
			Serial.println("reset");
			int * p = nullptr;
			*p = 0;
		}
		if (message[0] == 'B')
		{
			
		}  
		else if (message[0] == 'F')
		{
			
		}
		else if (message[0] == '#')
		{
			//преобразуем в 24 битное цветовое число
			uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
		
			//преобразуем 24 бит по 8 бит на канал 
			uint8_t r = abs((int)(0 + (rgb >> 16) & 0xFF));
			uint8_t g = abs((int)(0 + (rgb >>  8) & 0xFF));
			uint8_t b = abs((int)(0 + (rgb >>  0) & 0xFF));
			
			Serial.println("ColorPicker: " + r + g + b);
			FastLED.showColor(CRGB(r,g,b));
		}
		else if (message = "settings_request")
		{
			Serial.println("settings_request");
			webSocket.sendTXT(num, "settings_reply");
		}
		else
		{
			Serial.println("Unknown message");
		}
	} 
}

// функция получения типа файла
String getContentType(ESP8266WebServer & server, const String & filename)
{
		if(server.hasArg("download")) return "application/octet-stream";
		else if(filename.endsWith(".htm")) return "text/html";
		else if(filename.endsWith(".html")) return "text/html";
		else if(filename.endsWith(".css")) return "text/css";
		else if(filename.endsWith(".js")) return "application/javascript";
		else if(filename.endsWith(".png")) return "image/png";
		else if(filename.endsWith(".gif")) return "image/gif";
		else if(filename.endsWith(".jpg")) return "image/jpeg";
		else if(filename.endsWith(".ico")) return "image/x-icon";
		else if(filename.endsWith(".xml")) return "text/xml";
		else if(filename.endsWith(".pdf")) return "application/x-pdf";
		else if(filename.endsWith(".zip")) return "application/x-zip";
		else if(filename.endsWith(".gz")) return "application/x-gzip";
		return "text/plain";
}

// функция поиска файла в файловой системе
bool handleFileRead(ESP8266WebServer & server, String path)
{
	#ifdef DEBUG
		Serial.println("handleFileRead: " + path);
	#endif

	if (path == "/")
	{
		path += "index.html";
	}

	if (SPIFFS.exists(path))
	{
		File file = SPIFFS.open(path, "r");
		size_t sent = server.streamFile(file, getContentType(server, path));
		file.close();
		return true;
	}
	return false;
}*/