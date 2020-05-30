#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdint.h>
#include <unistd.h>
#include "cr_API.h"

// Global Variables
int PARTITION_SIZE = 536870912; //512 MB a bytes
int BLOCK_SIZE = 8192; // 8KB a bytes
int MAX_SIZE;

// Struct de archivos

crFILE *create_crFILE(char fileName[29], unsigned int indexLocation, uint64_t size, unsigned int references, unsigned int disk)
{
	crFILE *cr_File = malloc(sizeof(crFILE));
	cr_File->name = fileName;
	cr_File->indexLocation = indexLocation;
	cr_File->size = size;
	cr_File->references = references;
	cr_File->disk = disk;

	return cr_File;
}

void delete_crFILE(crFILE *cr_File)
{
	free(cr_File);
}
// Funciones Utiles

// returns the first block that is free
unsigned next_free_block(unsigned disk)
{
	FILE *file;
	char *buffer;
	// cada disco tiene 65536 bloques 512MB / 8KB
	int cont = 65536 * (disk - 1);
	int bitMapPointer = (disk - 1) * PARTITION_SIZE + BLOCK_SIZE;
	file = fopen(MOUNTED_DISK, "rb");
	fseek(file, bitMapPointer, SEEK_SET);
	buffer = malloc(sizeof(char) * BLOCK_SIZE);
	fread(buffer, sizeof(char), BLOCK_SIZE, file);
	for (int index = 0; index < BLOCK_SIZE; index++)
	{
		unsigned int byte = buffer[index];
		for (size_t i = 0; i < 8; i++)
		{
			unsigned int bit = byte & 0x080;
			bit >>= 7;
			if (bit == 0)
			{
				free(buffer);
				fclose(file);
				return (cont);
			}
			cont += 1;
			byte <<= 1;
		}
	}
	free(buffer);
	fclose(file);
	printf("No Free Blocks\n");
	return (0);
}

// Funciones Pedidas
void cr_mount(char *diskname)
{
	MOUNTED_DISK = diskname;
}
// imprime el block de directorio en bits
void cr_directory(unsigned disk)
{
	FILE *file;
	char *buffer;
	file = fopen(MOUNTED_DISK, "rb");
	int directoryStartByte = (disk - 1) * PARTITION_SIZE;
	fseek(file, directoryStartByte, SEEK_SET);
	buffer = malloc(sizeof(char) * BLOCK_SIZE);
	fread(buffer, sizeof(char), BLOCK_SIZE, file);

	// 8192 / 32 = 256 osea son 256 entradas
	for (int entry = 0; entry < 256; entry++)
	{
		for (int index = 0; index < 32; index++)
		{
			unsigned int byte = buffer[entry * 32 + index];
			for (size_t i = 0; i < 8; i++)
			{
				unsigned int bit = byte & 0x080;
				bit >>= 7;
				printf("%d", bit);
				byte <<= 1;
			}
			if (index % 32 == 31)
			{
				printf("\n");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
	free(buffer);
	fclose(file);
}

// src = https://stackoverflow.com/questions/55288402/printing-bytes-read-from-binary-file-at-a-certain-file-position-c
// saqué este código de internet espero que funcione como creo
void cr_bitmap(unsigned disk, bool hex)
{
	FILE *file;
	char *buffer;
	file = fopen(MOUNTED_DISK, "rb");
	int libre = 0;
	int ocupado = 0;
	if (file == NULL)
	{
		perror("Could not open file");
		exit(1);
	}
	if (disk > 0 && disk <= 4)
	{
		printf("DISK NUMBER: %d\n", disk);
		int bitMapPointer = (disk - 1) * PARTITION_SIZE + BLOCK_SIZE;
		fseek(file, bitMapPointer, SEEK_SET);
		buffer = malloc(sizeof(char) * BLOCK_SIZE);
		fread(buffer, sizeof(char), BLOCK_SIZE, file);
		if (hex)
		{
			for (int index = 0; index < BLOCK_SIZE; index++)
			{
				///
				unsigned int byte = buffer[index];
				for (size_t i = 0; i < 8; i++)
				{
					unsigned int bit = byte & 0x080;
					bit >>= 7;
					if (bit == 1)
						ocupado++;
					else
						libre++;
					byte <<= 1;
				}
				///
				printf("%02X", ((unsigned int)buffer[index]) & 0x0FF);
				if (index % 16 == 15)
				{
					printf("\n");
				}
				else
				{
					printf(" ");
				}
			}
			printf("Ocupados: %i\n", ocupado);
			printf("Libres: %i\n", libre);
			printf("Total: %i\n", libre + ocupado);
		}
		else
		{
			for (int index = 0; index < BLOCK_SIZE; index++)
			{

				unsigned int byte = buffer[index];
				for (size_t i = 0; i < 8; i++)
				{
					unsigned int bit = byte & 0x080;
					bit >>= 7;
					printf("%d", bit);
					if (bit == 1)
						ocupado++;
					else
						libre++;
					byte <<= 1;
				}
				if (index % 16 == 15)
				{
					printf("\n");
				}
				else
				{
					printf(" ");
				}
			}
			printf("Ocupados: %i\n", ocupado);
			printf("Libres: %i\n", libre);
			printf("Total: %i\n", libre + ocupado);
		}
		free(buffer);
	}
	else if (disk == 0)
	{
		for (unsigned z = 1; z < 5; z++)
		{
			printf("DISK NUMBER: %d\n", z);
			int bitMapPointer = (z - 1) * PARTITION_SIZE + BLOCK_SIZE;
			fseek(file, bitMapPointer, SEEK_SET);
			buffer = malloc(sizeof(char) * BLOCK_SIZE);
			fread(buffer, sizeof(char), BLOCK_SIZE, file);
			if (hex)
			{
				for (int index = 0; index < BLOCK_SIZE; index++)
				{
					///
					unsigned int byte = buffer[index];
					for (size_t i = 0; i < 8; i++)
					{
						unsigned int bit = byte & 0x080;
						bit >>= 7;
						if (bit == 1)
							ocupado++;
						else
							libre++;
						byte <<= 1;
					}
					///
					printf("%02X", ((unsigned int)buffer[index]) & 0x0FF);
					if (index % 16 == 15)
					{
						printf("\n");
					}
					else
					{
						printf(" ");
					}
				}
			}
			else
			{
				for (int index = 0; index < BLOCK_SIZE; index++)
				{

					unsigned int byte = buffer[index];
					for (size_t i = 0; i < 8; i++)
					{
						unsigned int bit = byte & 0x080;
						bit >>= 7;
						printf("%d", bit);
						if (bit == 1)
							ocupado++;
						else
							libre++;
						byte <<= 1;
					}
					if (index % 16 == 15)
					{
						printf("\n");
					}
					else
					{
						printf(" ");
					}
				}
			}
			free(buffer);
		}
		printf("Ocupados: %i\n", ocupado);
		printf("Libres: %i\n", libre);
		printf("Total: %i\n", libre + ocupado);
	}

	fclose(file);
}

int cr_exists(unsigned disk, char *filename)
{
	FILE *file;
	char *buffer;
	char entryFileName[29];
	int result;
	file = fopen(MOUNTED_DISK, "rb");
	int directoryStartByte = (disk - 1) * PARTITION_SIZE;
	fseek(file, directoryStartByte, SEEK_SET);
	buffer = malloc(sizeof(char) * BLOCK_SIZE);
	fread(buffer, sizeof(char), BLOCK_SIZE, file);

	// 8192 / 32 = 256 osea son 256 entradas
	for (int entry = 0; entry < 256; entry++)
	{
		for (int index = 3; index < 32; index++)
		{
			unsigned int byte = buffer[entry * 32 + index];
			entryFileName[index - 3] = byte;
		}
		result = strcmp(filename, entryFileName);
		if (result == 0)
		{
			fclose(file);
			free(buffer);
			return (1);
		}
	}
	fclose(file);
	free(buffer);
	return (0);
}

void cr_ls(unsigned disk)
{
	FILE *file;
	char *buffer;
	char entryFileName[29];
	int result;
	file = fopen(MOUNTED_DISK, "rb");
	printf("Files in DISK NUMBER: %d\n", disk);
	int directoryStartByte = (disk - 1) * PARTITION_SIZE;
	fseek(file, directoryStartByte, SEEK_SET);
	buffer = malloc(sizeof(char) * BLOCK_SIZE);
	fread(buffer, sizeof(char), BLOCK_SIZE, file);

	// 8192 / 32 = 256 osea son 256 entradas
	for (int entry = 0; entry < 256; entry++)
	{
		for (int index = 3; index < 32; index++)
		{
			unsigned int byte = buffer[entry * 32 + index];
			entryFileName[index - 3] = byte;
		}
		result = strcmp(entryFileName, "");
		if (result != 0)
		{
			printf("%s\n", entryFileName);
		}
	}
	free(buffer);
	fclose(file);
}

crFILE *cr_open(unsigned disk, char *filename, char mode)
{
	FILE *file;
	char *buffer;
	char entryFileName[29];
	int directoryEntry;
	unsigned int location;
	uint64_t size = 0;
	unsigned int references = 0;
	int result;

	if (mode == 'r')
	{
		if (cr_exists(disk, filename) == 1)
		{
			if (filename[1] == '/')
			{
				int softlinkDisk = (int)filename[0] - 48;
				if (softlinkDisk >= 0 && softlinkDisk <= 4)
				{
					disk = softlinkDisk;
					filename += 2;
					printf("Soft Link to %s in disk : %d\n", filename, disk);
				}
			}
		}
		if (cr_exists(disk, filename) == 1)
		{
			file = fopen(MOUNTED_DISK, "rb");
			int directoryStartByte = (disk - 1) * PARTITION_SIZE;
			fseek(file, directoryStartByte, SEEK_SET);
			buffer = malloc(sizeof(char) * BLOCK_SIZE);
			fread(buffer, sizeof(char), BLOCK_SIZE, file);

			//Obtiene el nombre y la entrada en el directorio del archivo
			for (int entry = 0; entry < 256; entry++)
			{
				for (int index = 3; index < 32; index++)
				{
					unsigned int byte = buffer[entry * 32 + index];
					entryFileName[index - 3] = byte;
				}
				result = strcmp(filename, entryFileName);
				if (result == 0)
				{
					directoryEntry = entry;
					break;
				}
			}
			// obtiene la ubicacion del index del archivo
			// el & 0x07F es para eliminar el bit de validez
			// No sé pq hay que hacer los otros & 0x0FF pero si no me da con puros 1 antes
			unsigned int byte1 = buffer[directoryEntry * 32] & 0x07F;
			unsigned int byte2 = buffer[directoryEntry * 32 + 1] & 0x0FF;
			unsigned int byte3 = buffer[directoryEntry * 32 + 2] & 0x0FF;
			location = (byte1 << 16) + (byte2 << 8) + byte3;
			free(buffer);
			int indexBlockPointer = location * BLOCK_SIZE;
			fseek(file, indexBlockPointer, SEEK_SET);
			buffer = malloc(sizeof(char) * BLOCK_SIZE);
			fread(buffer, sizeof(char), BLOCK_SIZE, file);
			//lee las referencias
			for (size_t i = 0; i < 4; i++)
			{
				unsigned int byte = buffer[i] & 0x0FF;
				references += byte;
				if (i != 3)
				{
					size <<= 8;
				}
			}
			// lee los 8 bytes de size
			for (size_t i = 4; i < 12; i++)
			{
				unsigned int byte = buffer[i] & 0x0FF;
				printf("byte %u\n", byte);
				size += byte;
				if (i != 11)
				{
					size <<= 8;
				}
			}
			crFILE *cr_File = create_crFILE(entryFileName, location, size, references, disk);
			printf("nombre : %s loc: %u, size %" PRIu64 " references : %u \n", cr_File->name, cr_File->indexLocation, cr_File->size, cr_File->references);
			free(buffer);
			fclose(file);
			return (cr_File);
		}
		else
		{
			//Sorry estaba muuy aburrido
			printf("Segmentation fault (core dumped)\n");
			sleep(1);
			printf("Broma....\n");
			printf("Archivo no existe\n");
			return (0);
		}
	}
	else if (mode == 'w')
	{
		if (cr_exists(disk, filename) == 0)
		{
			location = next_free_block(disk);
			crFILE *cr_File = create_crFILE(filename, location, size, references, disk);
			printf("nombre : %s loc: %u, size %" PRIu64 " references : %u \n", cr_File->name, cr_File->indexLocation, cr_File->size, cr_File->references);
			return (cr_File);
		}
		else
		{
			printf("Archivo ya existe\n");
			return (0);
		}
	}
	else
	{
		printf("Invalid Mode\n");
		return (0);
	}
}

int cr_hardlink(unsigned disk, char* orig, char* dest) {
  FILE *file;
  char *buffer;
  char *strNum;
  char fileData[3];
  char entryFileData[32];
  char entryFileName[29];
  unsigned int location;
  unsigned int references = 0;
  uint64_t size = 0;
  file = fopen(MOUNTED_DISK, "rb+");
  if (file == NULL) {
    perror("Could not open file");
    exit(1);
  }

  int directoryPointer = (disk-1) * PARTITION_SIZE;
  fseek(file, directoryPointer, SEEK_SET);
  buffer = malloc(sizeof(char) * BLOCK_SIZE);
  fread(buffer, sizeof(char), BLOCK_SIZE, file);

  for (int entry = 0; entry < 256; entry++) {
    for (int index = 0; index < 32; index++) {
      unsigned int byte = buffer[entry * 32 + index];
      if (index <= 2) {
        fileData[index] = byte;
      } else if (index > 2) {
        entryFileName[index-3] = byte;
      }
    }
    if (strcmp(orig, entryFileName) == 0) {
      unsigned int byte1 = buffer[entry*32] & 0x07F;
      unsigned int byte2 = buffer[entry*32+1] & 0x0FF;
      unsigned int byte3 = buffer[entry*32+2] & 0x0FF;
      location = (byte1 << 16) + (byte2 << 8) + byte3;
      int indexBlockPointer = location * BLOCK_SIZE;
      fseek(file, indexBlockPointer, SEEK_SET);
      strNum = malloc(sizeof(char) * BLOCK_SIZE);
      fread(strNum, sizeof(char), BLOCK_SIZE, file);

      for (size_t i = 0; i < 4; i++) {
        unsigned int byte = strNum[i] & 0x0FF;
        references += byte;
        if (i != 3) {
          size <<= 8;
        }
      }
      references++;
      fseek(file, indexBlockPointer, SEEK_SET);
      fwrite(&references, sizeof(int), 1, file);
      free(strNum);

      size_t j = 0;
      for (int i = 0; i < 3; i++) {
        entryFileData[j++] = fileData[i];
      }
      entryFileData[j] = 0;
      break;
    }
  }

  for (int entry = 0; entry < 256; entry++) {
    unsigned int firstByte = buffer[entry*32];
    unsigned int validity = (firstByte & 10000000) >> 7;
    if (validity == 0) {
      strcat(entryFileData, dest);
      int entryPointer = (disk-1) * PARTITION_SIZE + entry*32;
      fseek(file, entryPointer, SEEK_SET);
      fwrite(&entryFileData, sizeof(entryFileData), 1, file);
      fclose(file);
      break;
    }
  }
  free(buffer);
}

int cr_softlink(unsigned disk_orig, unsigned disk_dest, char *orig)
{

	if (cr_exists(disk_orig, orig))
	{
		char data[32];
		FILE *file;
		file = fopen(MOUNTED_DISK, "rb+");
		if (file == NULL)
		{
			perror("Could not open file");
			exit(1);
		}
		int directoryStartByte = (disk_dest - 1) * PARTITION_SIZE;
		fseek(file, directoryStartByte, SEEK_SET);
		char *buffer = malloc(sizeof(char) * BLOCK_SIZE);
		fread(buffer, sizeof(char), BLOCK_SIZE, file);
		int free_entry;
		//unsigned int first_bit;

		//Obtiene la primera entrada libre en disk_dest
		for (int entry = 0; entry < 256; entry++)
		{
			unsigned int first_byte = buffer[entry * 32];
			unsigned int first_bit = (first_byte & 0x0FF) >> 7;
			//char first_byte_c = first_byte + '0';
			if (first_bit == 0)
			{
				free_entry = entry;
				break;
			}
		}
		// llena los 32 bytes con la informacion
		char disk_num = disk_orig + '0';
		for (int i = 0; i < 32; i++)
			data[i] = NULL;
		unsigned int new_first_byte = 1 << 7;
		data[0] = new_first_byte;
		data[3] = disk_num;
		data[4] = '/';
		for (int i = 0; i < strlen(orig); i++)
			data[5 + i] = orig[i];
		//escribe en la entrada correspondiente
		int entryPointer = (disk_dest - 1) * PARTITION_SIZE + free_entry * 32;
		fseek(file, entryPointer, SEEK_SET);
		fwrite(&data, sizeof(data), 1, file);
		fclose(file);
		free(buffer);
	}
	else
	{
		printf("ERROR AL CREAR SOFTLINK: el archivo %s no está en disco %d\n", orig, disk_orig);
	}
	return 0;
}

int cr_read(crFILE *file_desc, void *buffer, int n_bytes) {
  FILE *file;
	int real_bytes;
  char *indexBlock;
	char *dataBlock;
	char *indirectionBlock;
	char *data;
	unsigned int location = 0;
	unsigned int indirectionLocation = 0;

  file = fopen(MOUNTED_DISK, "rb+");
  if (file == NULL) {
    perror("Could not open file");
    exit(1);
  }

	if (file_desc->size < n_bytes) {
		real_bytes = file_desc->size;
	} else {
		real_bytes = n_bytes;
	}
	data = malloc(real_bytes);

  int indexPointer = file_desc->indexLocation * BLOCK_SIZE;
  fseek(file, indexPointer, SEEK_SET);
  indexBlock = malloc(sizeof(char) * BLOCK_SIZE);
  fread(indexBlock, sizeof(char), BLOCK_SIZE, file);
	for (int ptr = 0; ptr < 2044; ptr++) {
		for (int i = 0; i < 4; i++) {
			unsigned int byte = indexBlock[ptr * 4 + 11 + i] & 0x0FF;
			location += byte;
		}
		int dataPointer = location * BLOCK_SIZE;
		location = 0;
		fseek(file, dataPointer, SEEK_SET);
		dataBlock = malloc(sizeof(char) * BLOCK_SIZE);
		fread(dataBlock, sizeof(char), BLOCK_SIZE, file);
		if ((ptr + 1) * BLOCK_SIZE <= real_bytes) {
			strcat(data, dataBlock);
		} else if ((ptr + 1) * BLOCK_SIZE - real_bytes < BLOCK_SIZE) {
			int difference = (ptr + 1) * BLOCK_SIZE - real_bytes;
			char *differenceData;
			size_t j = 0;
      for (int i = 0; i < difference; i++) {
        differenceData[j++] = dataBlock[i];
      }
      differenceData[j] = 0;
			strcat(data, differenceData);
			break;
		} else {
			break;
		}
		free(dataBlock);
	}
	if (2044 * BLOCK_SIZE < real_bytes) {
		for (size_t i = 4; 0 < i; i--) {
			unsigned int byte = indexBlock[BLOCK_SIZE - i] & 0x0FF;
			indirectionLocation += byte;
		}
		int indirectionPointer = indirectionLocation * BLOCK_SIZE;
		fseek(file, indirectionPointer, SEEK_SET);
		indirectionBlock = malloc(sizeof(char) * BLOCK_SIZE);
		fread(indirectionBlock, sizeof(char), BLOCK_SIZE, file);
		// Hay punteros 2048 en un bloque de indireccionamiento simple
		for (int ptr = 0; ptr < 2048; ptr++) {
			for (size_t i = 0; i < 4; i++) {
				unsigned int byte = indirectionBlock[ptr * 4 + i] & 0x0FF;
				location += byte;
			}
			int dataPointer = location * BLOCK_SIZE;
			location = 0;
			fseek(file, dataPointer, SEEK_SET);
			dataBlock = malloc(sizeof(char) * BLOCK_SIZE);
			fread(dataBlock, sizeof(char), BLOCK_SIZE, file);
			if (2044 * BLOCK_SIZE + (ptr + 1) * BLOCK_SIZE <= real_bytes) {
				strcat(data, dataBlock);
				free(dataBlock);
			} else if (2044 * BLOCK_SIZE + (ptr + 1) * BLOCK_SIZE - real_bytes < BLOCK_SIZE) {
				int difference = 2044 * BLOCK_SIZE + (ptr + 1) * BLOCK_SIZE - real_bytes;
				char *differenceData;
				size_t j = 0;
				for (int i = 0; i < difference; i++) {
					differenceData[j++] = dataBlock[i];
				}
				differenceData[j] = 0;
				strcat(data, differenceData);
				break;
			} else {
				break;
			}
		}
	}
	free(indexBlock);
	free(indirectionBlock);
	return(real_bytes);
}

int cr_close(crFILE *file_desc) {
	free(file_desc);
	return (0);
}

int cr_write(crFILE *file_desc, void *buffer, int nbytes) {
	//next_free_block(disk)
	char entryFileData[32] = {0};
	//int contador = 0;
	FILE *file = fopen(MOUNTED_DISK, "rb+");
	//////
	char *buffer_aux;
	char fileData[3];
	int directoryPointer = (file_desc->disk - 1) * PARTITION_SIZE;
	fseek(file, directoryPointer, SEEK_SET);
	buffer_aux = malloc(sizeof(char) * BLOCK_SIZE);
	fread(buffer_aux, sizeof(char), BLOCK_SIZE, file);

	//int indexBlockPointer = file_desc->indexLocation * BLOCK_SIZE;
	for (int entry = 0; entry < 256; entry++)
	{
		unsigned int firstByte = buffer_aux[entry * 32];
		unsigned int validity = (firstByte & 10000000) >> 7;
		if (validity == 0) //entrada de directorio libre
		{
			unsigned int validez = 1;
			unsigned int indexBlock = file_desc->indexLocation;

			fileData[0] = validez << 7 | indexBlock >> 16;
			fileData[1] = indexBlock >> 8;
			fileData[2] = indexBlock;
			for (int i = 0; i < 3; i++)
			{
				entryFileData[i] = fileData[i];
			}
			for (int i = 3; i < 32; i++)
			{
				if (file_desc->name[i - 3])
				{
					entryFileData[i] = file_desc->name[i - 3];
				}
				else
					break;
			}
			//esto es para modificar bloque directorio
			int entryPointer = (file_desc->disk - 1) * PARTITION_SIZE + entry * 32;
			fseek(file, entryPointer, SEEK_SET);
			fwrite(&entryFileData, sizeof(entryFileData), 1, file);
			fclose(file);
			break;
		}
	}
	free(buffer_aux);
	/////////

	// int lista_punteros[2044] = {0};
	// for (int i = 12; i < 8188;) //8188
	// {
	// 	uint64_t puntero = 0;
	// 	for (int j = 0; j < 4; j++)
	// 	{
	// 		//printf("%d ", buffer_aux[i + j] & 0x0FF); //corresponde a 4 bytes de ubicacion de bloques
	// 		puntero += buffer_aux[i + j] & 0x0FF;
	// 		if (j != 3)
	// 		{
	// 			puntero <<= 8;
	// 		}
	// 	}
	// 	lista_punteros[contador] = puntero;
	// 	if (puntero > 0)
	// 		contador++;
	// 	//printf("\npuntero %ld\n", puntero);

	// 	i += 4;
	// 	//printf("\n");
	// }
	// printf("contador: %d\n", contador);
	return 0;
}
