#pragma once
#include "fileparser.h"
#include <cstring>

#define MASK(x) x&(0b00000011)


typedef struct
{
	BYTE b[4];
}SPLIT;


class BMP
{
public:
	BMP(LPCSTR lpFileName) : hFile(lpFileName)
	{}

	size_t size() const
	{
		return static_cast<size_t>(hFile.read<UINT32>(2) );
	}
	size_t max_message_len() const
	{
		return ((static_cast<size_t>(hFile.read<UINT32>(0x02)) - static_cast<size_t>(hFile.read<UINT32>(0x0A))) / 4) - sizeof(size_t);
	}

	static SPLIT splitByte(BYTE byte)
	{
		SPLIT rep;
		rep.b[0] = MASK(byte);
		byte >>= 2;
		rep.b[1] = MASK(byte);
		byte >>= 2;
		rep.b[2] = MASK(byte);
		byte >>= 2;
		rep.b[3] = MASK(byte);
		return rep;
	}

	static BYTE colapseByte(SPLIT splt)
	{
		BYTE rep = 0;
		rep += splt.b[3];
		rep <<= 2;
		rep += splt.b[2];
		rep <<= 2;
		rep += splt.b[1];
		rep <<= 2;
		rep += splt.b[0];
		return rep;
	}

	void HideMessage(LPCSTR message)
	{
		UINT32 addr = hFile.read<UINT32>(0x000A);
		size_t len = strlen(message)+1;
		
		if (len > max_message_len())
			len = max_message_len();


		SPLIT splt,data;

		//writing message len
		size_t clen = len; //save len to avoid modif
		for (size_t i = 0; i < sizeof(size_t); i++)
		{
			//splitint size_t to byte 
			splt = splitByte(clen % 0xff);
			clen >>= 8;
			data = hFile.read<SPLIT>(addr + 4 * i);
			for (size_t j = 0; j < 4; j++)
			{
				data.b[j] >>= 2;
				data.b[j] <<= 2;
				data.b[j] += splt.b[j];
			}
			hFile.write<SPLIT>(addr + 4 * i, data);
		}

		//fixing offset
		addr += sizeof(size_t)*4 ;
		//writing message
		for (size_t i = 0; i < len; i++)
		{
			splt = splitByte(message[i]);
			data = hFile.read<SPLIT>(addr + 4 * i);
			for (size_t j = 0; j < 4; j++)
			{
				data.b[j] >>= 2;
				data.b[j] <<= 2;
				data.b[j] += splt.b[j];
			}
			hFile.write<SPLIT>(addr + 4 * i, data);
		}
	}

	size_t ReadMessage(char* output) // return message len
	{
		UINT32 addr = hFile.read<UINT32>(0x000A);

		SPLIT splt, data;

		size_t len = 0;


		//retrieving len
		for (size_t i = 0; i < sizeof(size_t); i++)
		{
			data = hFile.read<SPLIT>(addr + 4 * i);
			
			for (size_t j = 0; j < 4; j++)
			{
				splt.b[j] = MASK(data.b[j]);
			}

			len += (colapseByte(splt) << (i * 8));

		}

		//fixing offset
		addr += sizeof(size_t) * 4;

		//reading message
		for (size_t i = 0; i < len; i++)
		{
			data = hFile.read<SPLIT>(addr + 4 * i);
			for (size_t j = 0; j < 4; j++)
			{
				splt.b[j] = MASK(data.b[j]);
			}
			output[i] = colapseByte(splt);
		}
		return len;
	}

protected:
	File hFile;



};