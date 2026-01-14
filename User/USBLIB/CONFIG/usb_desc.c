/********************************** (C) COPYRIGHT *******************************
* File Name          : usb_desc.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2022/07/15
* Description        : USB Descriptors.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/ 
#include "usb_lib.h"
#include "usb_desc.h"

/* USB Device Descriptors */
const uint8_t  USBD_DeviceDescriptor[] = { 
    USBD_SIZE_DEVICE_DESC,           // bLength
    0x01,                            // bDescriptorType (Device)
    0x00, 0x02,                      // bcdUSB 2.0
    0x00,                            // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,                            // bDeviceSubClass 
    0x00,                            // bDeviceProtocol 
    DEF_USBD_UEP0_SIZE,              // bMaxPacketSize0 8
    0x86, 0x1A,                      // idVendor 0x1A86
    0x07, 0xFE,                      // idProduct 0xFE07
    0x00, 0x01,                      // bcdDevice 2.00
    0x01,                            // iManufacturer (String Index)
    0x02,                            // iProduct (String Index)
    0x03,                            // iSerialNumber (String Index)
    0x01,                            // bNumConfigurations 1
};

/* USB Configration Descriptors */
const uint8_t  USBD_ConfigDescriptor[] = { 
    /* Configuration Descriptor */
    0x09,                           // bLength
    0x02,                           // bDescriptorType
    USBD_SIZE_CONFIG_DESC & 0xFF, USBD_SIZE_CONFIG_DESC >> 8, // wTotalLength
    0x01,                           // bNumInterfaces
    0x01,                           // bConfigurationValue
    0x03,                           // iConfiguration (String Index)
    0x80,                           // bmAttributes Remote Wakeup
    0x23,                           // bMaxPower 70mA

    /* Interface Descriptor */
    0x09,                           // bLength
    0x04,                           // bDescriptorType (Interface)
    0x00,                           // bInterfaceNumber 0
    0x00,                           // bAlternateSetting
    0x02,                           // bNumEndpoints 2
    0x03,                           // bInterfaceClass
    0x01,                           // bInterfaceSubClass   boot=1; no boot=0
    0x01,                           // bInterfaceProtocol   keyboard=1; mouse=2
    0x00,                           // iInterface (String Index)

    /* HID Descriptor */
    0x09,                           // bLength
    0x21,                           // bDescriptorType
    0x11, 0x01,                     // bcdHID
    0x00,                           // bCountryCode
    0x01,                           // bNumDescriptors
    0x22,                           // bDescriptorType
    USBD_SIZE_REPORT_DESC_26KRO & 0xFF, USBD_SIZE_REPORT_DESC_26KRO >> 8, // wDescriptorLength

    /* Endpoint Descriptor */
    0x07,                           // bLength
    0x05,                           // bDescriptorType
    0x01,                           // bEndpointAddress: OUT Endpoint 1
    0x03,                           // bmAttributes
    0x40, 0x00,                     // wMaxPacketSize
    0x01,                           // bInterval: 1mS
    
    /* Endpoint Descriptor */
    0x07,                           // bLength
    0x05,                           // bDescriptorType
    0x82,                           // bEndpointAddress: IN Endpoint 2
    0x03,                           // bmAttributes
    0x06, 0x00,                     // wMaxPacketSize
    0x01,                           // bInterval: 1mS
};

/* USB String Descriptors */
const uint8_t USBD_StringLangID[USBD_SIZE_STRING_LANGID] = {
	USBD_SIZE_STRING_LANGID,
	USB_STRING_DESCRIPTOR_TYPE,
	0x09,
	0x04 
};

/* USB Device String Vendor */
const uint8_t USBD_StringVendor[USBD_SIZE_STRING_VENDOR] = {
	USBD_SIZE_STRING_VENDOR,    
	USB_STRING_DESCRIPTOR_TYPE,           
	'w',0,'c',0,'h',0,'.',0,'c',0,'n',0
};

/* USB Device String Product */
const uint8_t USBD_StringProduct[USBD_SIZE_STRING_PRODUCT] = {
	USBD_SIZE_STRING_PRODUCT,         
	USB_STRING_DESCRIPTOR_TYPE,        
    'C', 0, 'H', 0, '3', 0, '2', 0, 'V', 0, '2', 0 , '0', 0, 'x', 0
};

/* USB Device String Serial */
uint8_t USBD_StringSerial[USBD_SIZE_STRING_SERIAL] = {
	USBD_SIZE_STRING_SERIAL,          
	USB_STRING_DESCRIPTOR_TYPE,                   
	'0', 0, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0 , '6', 0, '7', 0, '8', 0, '9', 0
};

/* HID Report Descriptor */
const uint8_t USBD_HidRepDesc[USBD_SIZE_REPORT_DESC] =
{
    0x06, 0x00, 0xFF,               // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,                     // Usage (0x01)
    0xA1, 0x01,                     // Collection (Application)
    0x09, 0x02,                     //   Usage (0x02)
    0x26, 0xFF, 0x00,               //   Logical Maximum (255)
    0x75, 0x08,                     //   Report Size (8)
    0x15, 0x00,                     //   Logical Minimum (0)
    0x95, 0x40,                     //   Report Count (64)
    0x81, 0x06,                     //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x02,                     //   Usage (0x02)
    0x15, 0x00,                     //   Logical Minimum (0)
    0x26, 0xFF, 0x00,               //   Logical Maximum (255)
    0x75, 0x08,                     //   Report Size (8)
    0x95, 0x40,                     //   Report Count (64)
    0x91, 0x06,                     //   Output (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                           // End Collection
};

/* 官方例程6键无冲报告描述符 */
const uint8_t usbd_6kro_report[62] =
{
    0x05, 0x01,                     // Usage Page (Generic Desktop)
    0x09, 0x06,                     // Usage (Keyboard)
    0xA1, 0x01,                     // Collection (Application)
    0x05, 0x07,                     // Usage Page (Key Codes)
    0x19, 0xE0,                     // Usage Minimum (224)
    0x29, 0xE7,                     // Usage Maximum (231)
    0x15, 0x00,                     // Logical Minimum (0)
    0x25, 0x01,                     // Logical Maximum (1)
    0x75, 0x01,                     // Report Size (1)
    0x95, 0x08,                     // Report Count (8)
    0x81, 0x02,                     // Input (Data,Variable,Absolute)
    0x95, 0x01,                     // Report Count (1)
    0x75, 0x08,                     // Report Size (8)
    0x81, 0x01,                     // Input (Constant)
    0x95, 0x03,                     // Report Count (3)
    0x75, 0x01,                     // Report Size (1)
    0x05, 0x08,                     // Usage Page (LEDs)
    0x19, 0x01,                     // Usage Minimum (1)
    0x29, 0x03,                     // Usage Maximum (3)
    0x91, 0x02,                     // Output (Data,Variable,Absolute)
    0x95, 0x05,                     // Report Count (5)
    0x75, 0x01,                     // Report Size (1)
    0x91, 0x01,                     // Output (Constant,Array,Absolute)
    0x95, 0x06,                     // Report Count (6)
    0x75, 0x08,                     // Report Size (8)
    0x26, 0xFF, 0x00,               // Logical Maximum (255)
    0x05, 0x07,                     // Usage Page (Key Codes)
    0x19, 0x00,                     // Usage Minimum (0)
    0x29, 0x91,                     // Usage Maximum (145)
    0x81, 0x00,                     // Input(Data,Array,Absolute)
    0xC0                            // End Collection
};


/* 自定义26键无冲键盘描述符 */
/* 输入数组 6 字节，后 4 字节每一位由 a 到 z */
const uint8_t usbd_26kro_report[73] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x04,                    //   USAGE_MINIMUM (Keyboard a and A)
    0x29, 0x1d,                    //   USAGE_MAXIMUM (Keyboard z and Z)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x1a,                    //   REPORT_COUNT (26)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x06,                    //   REPORT_SIZE (6)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0xc0                           // END_COLLECTION
};

