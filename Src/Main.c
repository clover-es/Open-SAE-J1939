/*
 * Main.c
 *
 *  Created on: 16 juli 2021
 *      Author: Daniel Mårtensson
 */

#include "stdlib.h"
#include "stdio.h"

#include "ISO 11783/ISO 11783-7 Application Layer/ISO_11783-7_Application_Layer.h"
#include "Open SAE J1939/Open_SAE_J1939.h"
#include "SAE J1939-71 Application Layer/SAE_J1939-71_Application_Layer.h"
#include "SAE J1939-73 Diagnostics Layer/SAE_J1939-73_Diagnostics_Layer.h"
#include "SAE J1939-81 Network Management Layer/SAE_J1939-81_Network_Management_Layer.h"

int main() {

	/* Create our J1939 structure with two ECU */
	J1939 j1939_1 = {0};
	J1939 j1939_2 = {0};

	/* Set the ECU address */
	j1939_1.this_ECU_address = 0x80;												/* From 0 to 253 because 254 = error address and 255 = broadcast address */
	j1939_2.this_ECU_address = 0x90;

	/* This is important because length_of_each_field is not a SAE J1939 standard. The software need to know how large field it is from the beginning */
	j1939_2.from_other_ecu_component_identification.length_of_each_field = 20;

	/* Set the Component Identification */
	j1939_1.this_component_identification.length_of_each_field = 20;
	char component_product_date[20] = "2021-07-20";
	char component_model_name[20] = "200X-PPT";
	char component_serial_number[20] = "1243-13244-1244";
	char component_unit_name[20] = "Unit 4K";
	for(uint8_t i = 0; i < 20; i++){
		j1939_1.this_component_identification.component_product_date[i] = (uint8_t) component_product_date[i];
		j1939_1.this_component_identification.component_model_name[i] = (uint8_t) component_model_name[i];
		j1939_1.this_component_identification.component_serial_number[i] = (uint8_t) component_serial_number[i];
		j1939_1.this_component_identification.component_unit_name[i] = (uint8_t) component_unit_name[i];
	}

	/* Request Software Identification from ECU 2 to ECU 1 */
	SAE_J1939_Send_Request(&j1939_2, 0x80, PGN_COMPONENT_IDENTIFICATION);

	/* Response request from ECU 1 perspective */
	Open_SAE_J1939_Listen_For_Messages(&j1939_1);

	/* Read response request from ECU 1 to ECU 2 */
	for(uint8_t i = 0; i < 15; i++)
		Open_SAE_J1939_Listen_For_Messages(&j1939_2);

	/* Display what ECU 2 got */
	printf("Length of each field = %i\nComponent product date = %s\nComponent model name = %s\nComponent serial number = %s\nComponent unit name = %s\n"
			,j1939_2.from_other_ecu_component_identification.length_of_each_field
			,j1939_2.from_other_ecu_component_identification.component_product_date
			,j1939_2.from_other_ecu_component_identification.component_model_name
			,j1939_2.from_other_ecu_component_identification.component_serial_number
			,j1939_2.from_other_ecu_component_identification.component_unit_name);

	return 0;
}
