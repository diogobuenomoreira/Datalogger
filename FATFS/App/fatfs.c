/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"
#include "usart.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
static FATFS FatFS;    /* File system object for USER logical drive */
static FIL file;       /* File object for USER */

/* USER CODE BEGIN Variables */
static void SD_Card_Mount(void);
static void SD_Card_Unmount(void);
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
static void SD_Card_Mount(void)
{
	FRESULT fres;

	fres = f_mount(&FatFS, "", 1);    //1=mount now
	if (fres != FR_OK)
	{
		DEBUG_PRINTF("No SD Card found : (%i)\r\n", fres);
	}
	else
	{
		DEBUG_MESSAGE("SD Card Mounted Successfully!!!\r\n");
	}
}

static void SD_Card_Unmount(void)
{
	FRESULT fres;

	fres = f_mount(NULL, "", 0);
	if (fres != FR_OK)
	{
		DEBUG_PRINTF("SD Card Unmounted Failed: (%i) \r\n", fres);
	}
	else
	{
		DEBUG_MESSAGE("SD Card Unmounted Successfully!!!\r\n");
	}
}

void SD_Card_Open(void)
{
    FRESULT fres;

    SD_Card_Mount();

    fres = f_open(&file, "datalogger.bin", FA_WRITE | FA_OPEN_APPEND | FA_CREATE_ALWAYS);
    if (fres != FR_OK)
    {
        DEBUG_PRINTF("File creation/open Error : (%i)\r\n", fres);
    }
}

void SD_Card_Close(void)
{
	FRESULT fres;

	fres = f_close(&file);
	if(fres != FR_OK)
	{
		DEBUG_PRINTF("File close Error : (%i)\r\n", fres);
	}
	SD_Card_Unmount();
}

void SD_Card_Write(const uint8_t* data, uint32_t length)
{
	FRESULT fres;
	UINT bytes_written;

	fres = f_write(&file, data, (UINT)length, &bytes_written);
	if((fres != FR_OK) || (bytes_written != length))
	{
		DEBUG_PRINTF("File write Error : (%i)\r\n", fres);
	}
	else
	{
		DEBUG_PRINTF("Written %i bytes \r\n", bytes_written);
	}
	f_sync(&file);
}

/* USER CODE END Application */
