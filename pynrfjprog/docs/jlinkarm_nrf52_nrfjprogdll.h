/*
 * Copyright (c) 2010 - 2018, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef JLINKARM_NRF52_NRFJPROGDLL_H
#define JLINKARM_NRF52_NRFJPROGDLL_H


#include <stdint.h>
#include "DllCommonDefinitions.h"


#if defined(__cplusplus)
extern "C" {
#endif


/**
 * @brief   Opens the JLinkARM DLL and sets the log callback. Prepares the dll for work with an nRF52 device.
 *
 * @details This function opens the JLinkARM DLL using the received path. The path should include the name of the DLL
 *          itself (i.e. "JLinkARM.dll"). Only JLinkARM DLLs whose versions are greater than a minimum version will be
 *          accepted. The minimum version for the JLinkARM DLL is defined in MIN_JLINK_MAJOR_VERSION and
 *          MIN_JLINK_MINOR_VERSION defines. The log callback may be NULL. In that case no logging mechanism is
 *          provided. The msg_callback typedef is defined in DllCommonDefinitions.h. To close the dll, see NRFJPROG_close()
 *          function.
 *
 * @pre     Before the execution of this function, the dll must not be open. To close the dll, see NRFJPROG_close_dll()
 *          function.
 *
 * @post    After the execution of this function, the JLINKARM DLL pointers will be loaded and some memory reserved. To
 *          unload the pointers and free the memory, see NRFJPROG_close_dll() function.
 * @post    After the execution of this function, the instance variable pointed to by instance_ptr will refer to the
 *          opened nrfjprog session.
 *
 * @param   instance_ptr                        Pointer to variable that will store the working instance of nrfjprog.
 * @param   jlink_path                          Path to the JLinkARM DLL. Does not support unicode paths. If NULL or
 *                                              nullptr, nrfjprog will attempt to find the newest installed J-Link Dll.
 * @param   log_cb                              Callback for reporting informational and error messages. May be NULL.
 * @param   log_param                           User-selectable value that will be passed back when calling log_cb in the _ex version of this function.
 *                                              log_param will not be dereferenced. May be NULL.
 * @param   family                              Defines the device family the next commands are going to be called to.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance_ptr is a NULL pointer.
 * @retval  INVALID_OPERATION                   The NRFJPROG_open_dll() function has already been called.
 * @retval  INVALID_PARAMETER                   The provided device family is not supported by this DLL.
 * @retval  JLINKARM_DLL_TOO_OLD                The version of JLinkARM is lower than the minimum version required.
 * @retval  JLINKARM_DLL_NOT_FOUND              The jlink_path did not yield a usable DLL, or the automatic search
 *                                              failed.
 * @retval  JLINKARM_DLL_COULD_NOT_BE_OPENED    An error occurred while opening the JLinkARM DLL.
 *                                              A required function could not be loaded from the DLL.
 */
nrfjprogdll_err_t NRFJPROG_open_dll_inst(nrfjprog_inst_t * instance_ptr,
                                         const char * jlink_path,
                                         msg_callback_ex * log_cb,
                                         void * callback_param,
                                         device_family_t family);
nrfjprogdll_err_t NRFJPROG_open_dll(const char * jlink_path, msg_callback * log_cb, device_family_t family);

/**
 * @brief   Closes and frees the JLinkARM DLL.
 *
 * @details Closes and frees the JLinkARM DLL. This function needs to be called before exiting if NRFJPROG_open_dll()
 *          has been called. After the execution of this function, the device CPU will not change its state from running
 *          or halted.
 *
 * @post    After the execution of this function, the JLINKARM DLL function pointers will be unloaded and the reserved
 *          memory freed. To open the dll, see NRFJPROG_open_dll() function.
 * @post    After the execution of this function, the device will not be in debug interface mode. To enter debug
 *          interface mode, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the PC will be disconnected from an emulator. To connect to an
 *          emulator, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @post    After the execution of this function, the emulator will be disconnected from the device. To connect to the
 *          device, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the trace and debug resources will be switched off. To switch on the
 *          debug resources, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the instance pointer will be cleared, and the memory associated with
 *          the instace will be freed.
 *
 * @param   instance_ptr                        Pointer to variable that will be closed.
 */
void NRFJPROG_close_dll_inst(nrfjprog_inst_t * instance_ptr);
void NRFJPROG_close_dll(void);


/**
 * @brief   Returns the JLinkARM DLL version.
 *
 * @details Returns the JLinkARM DLL version that has been opened in with the NRFJPROG_open_dll() function.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll()
 *          function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   major                               Pointer for storing of dll major version.
 * @param   minor                               Pointer for storing of dll minor version.
 * @param   revision                            Pointer for storing of dll revision.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The major parameter is NULL.
 *                                              The minor parameter is NULL.
 *                                              The revision parameter is NULL.
 */
nrfjprogdll_err_t NRFJPROG_dll_version_inst(nrfjprog_inst_t instance, uint32_t * major, uint32_t * minor, char * revision);
nrfjprogdll_err_t NRFJPROG_dll_version(uint32_t * major, uint32_t * minor, char * revision);

/**
 * @brief   Returns the path to the JLinkARM shared library.
 *
 * @details Returns the path to the JLinkARM DLL that has been opened with the NRFJPROG_open_dll() function.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll()
 *          function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   buffer                              Buffer to store the path.
 * @param   buffer_size                         Size of buffer
 * @param   bytes_copied                        Pointer to variable that will hold the amount of bytes copied to buffer.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The major parameter is NULL.
 *                                              The minor parameter is NULL.
 *                                              The revision parameter is NULL.
 */
nrfjprogdll_err_t NRFJPROG_get_jlink_path_inst(nrfjprog_inst_t instance,
                                               char * buffer,
                                               const size_t buffer_size,
                                               size_t * bytes_copied);
nrfjprogdll_err_t NRFJPROG_get_jlink_path(char * buffer, const size_t buffer_size, size_t * bytes_copied);


/**
 * @brief   Checks if the JLinkARM DLL is open.
 *
 * @details For NRFJPROG_is_dll_open_inst():
 *          Checks if the instance variable provided points to a valid and open nrfjprogdll instance.
 *
 *          For NRFJPROG_is_dll_open():
 *          NRFJPROG_open_dll() function has been called since the last call to NRFJPROG_close_dll().
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   opened                              Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The opened parameter is NULL.
 */
nrfjprogdll_err_t NRFJPROG_is_dll_open_inst(nrfjprog_inst_t instance, bool * opened);
nrfjprogdll_err_t NRFJPROG_is_dll_open(bool * opened);


/**
 * @brief   Enumerates all comports connected to a given Segger debug probe
 *
 * @details This function finds all COM ports hosted by a given debug probe.
 *          The number of com ports found is written into the num_com_ports parameter. It also copies
 *          up to com_ports_len com_port_info_t objects into the com_ports array parameter.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   serial_number                       Serial number of the debug probe to find the COM port of.
 * @param   com_ports                           Array in which to store the enumerated COM ports.
 * @param   com_ports_len                       Number of com_port_info_t values that can be stored in the
 *                                              com_ports array.
 * @param   num_com_ports                       The number of COM ports that were discovered.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INTERNAL_ERROR                      An internal error has occured.
 * @retval  INVALID_PARAMETER                   The com_ports parameter is NULL.
 *                                              The com_ports_len parameter is 0.
 *                                              The num_available parameter is NULL.
**/
nrfjprogdll_err_t NRFJPROG_enum_emu_com_inst(nrfjprog_inst_t instance, const uint32_t serial_number, com_port_info_t com_ports[], const uint32_t com_ports_len, uint32_t * num_com_ports);
nrfjprogdll_err_t NRFJPROG_enum_emu_com(const uint32_t serial_number, com_port_info_t com_ports[], const uint32_t com_ports_len, uint32_t * num_com_ports);


/**
 * @brief   Enumerates the serial numbers of connected USB J-Link emulators.
 *
 * @details This function asks the JLinkARM DLL how many USB J-Link emulators are connected to
 *          the PC, and writes that value into the num_available parameter. It also copies
 *          up to serial_numbers_len serial numbers into the serial_numbers array parameter.
 *          The function can be called with serial_numbers set to NULL and serial_numbers_len
 *          set to zero to obtain the number of connected emulators.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   serial_numbers                      Array in which to store the enumerated serial numbers.
 * @param   serial_numbers_len                  Number of uint32_t values that can be stored in the
 *                                              serial_numbers array (may be zero).
 * @param   num_available                       The number of serial numbers that were enumerated.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  INVALID_PARAMETER                   The serial_numbers parameter is NULL but serial_numbers_len is > 0.
 *                                              The num_available parameter is NULL.
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 */
nrfjprogdll_err_t NRFJPROG_enum_emu_snr_inst(nrfjprog_inst_t instance, uint32_t serial_numbers[], uint32_t serial_numbers_len, uint32_t * num_available);
nrfjprogdll_err_t NRFJPROG_enum_emu_snr(uint32_t serial_numbers[], uint32_t serial_numbers_len, uint32_t * num_available);


/**
 * @brief   Checks if the emulator has an established connection with Segger emulator/debugger.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   is_pc_connected_to_emu              Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The is_connected_to_emu pointer is NULL.
 */
nrfjprogdll_err_t NRFJPROG_is_connected_to_emu_inst(nrfjprog_inst_t instance, bool * is_pc_connected_to_emu);
nrfjprogdll_err_t NRFJPROG_is_connected_to_emu(bool * is_pc_connected_to_emu);


/**
 * @brief   Connects to a given emulator/debugger.
 *
 * @details This function connects to serial_number emulator and sets the SWD communication speed at clock_speed_in_khz.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must not be established. To disconnect from an emulator, see NRFJPROG_disconnect_from_emu() function.
 * @pre     Before the execution of this function, the emulator must be physically connected to a powered board.
 *
 * @post    After the execution of this function, the PC will be connected to an emulator. To disconnect to the emulator, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   serial_number                       Serial number of the emulator to connect to.
 * @param   clock_speed_in_khz                  Speed for the SWD communication. It must be between JLINKARM_SWD_MIN_SPEED_KHZ
 *                                              and JLINKARM_SWD_MAX_SPEED_KHZ defined in DllCommonDefinitions.h. If the emulator
 *                                              does not support the input clock_speed, the emulators maximum supported speed
 *                                              will be used.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() has already been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  LOW_VOLTAGE                         Low voltage was detected at the target device.
 * @retval  INVALID_PARAMETER                   The clock_speed_in_khz parameter is not within limits.
 * @retval  EMULATOR_NOT_CONNECTED              The serial_number emulator is not connected to the PC.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The target debug port IDR does not match the expected value of an nRF52 SoC.
 */
nrfjprogdll_err_t NRFJPROG_connect_to_emu_with_snr_inst(nrfjprog_inst_t instance, uint32_t serial_number, uint32_t clock_speed_in_khz);
nrfjprogdll_err_t NRFJPROG_connect_to_emu_with_snr(uint32_t serial_number, uint32_t clock_speed_in_khz);


/**
 * @brief   Connects to an emulator/debugger.
 *
 * @details This function connects to an available emulator and sets the SWD communication speed at clock_speed_in_khz.
 *          If more than one emulator is available, a pop-up window will appear to make a selection.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must not be established. To disconnect from an emulator, see NRFJPROG_disconnect_from_emu() function.
 * @pre     Before the execution of this function, the emulator must be physically connected to a powered board.
 *
 * @post    After the execution of this function, the PC will be connected to an emulator. To disconnect to the emulator, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   clock_speed_in_khz                  Speed for the SWD communication. It must be between JLINKARM_SWD_MIN_SPEED_KHZ
 *                                              and JLINKARM_SWD_MAX_SPEED_KHZ defined in DllCommonDefinitions.h. If the emulator
 *                                              does not support the input clock_speed, the emulators maximum supported speed
 *                                              will be used.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() has already been called.
 * @retval  NO_EMULATOR_CONNECTED               There is no emulator connected to the PC.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  LOW_VOLTAGE                         Low voltage was detected at the target device.
 * @retval  INVALID_PARAMETER                   The clock_speed_in_khz parameter is not within limits.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The target debug port IDR does not match the expected value of an nRF52 SoC.
 */
nrfjprogdll_err_t NRFJPROG_connect_to_emu_without_snr_inst(nrfjprog_inst_t instance, uint32_t clock_speed_in_khz);
nrfjprogdll_err_t NRFJPROG_connect_to_emu_without_snr(uint32_t clock_speed_in_khz);


/**
 * @brief   Reads the serial number of the emulator connected to.
 *
 * @details Reads the serial number of the emulator connected to.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   serial_number                       Pointer to a variable that will hold the serial number.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The serial_number pointer is NULL.
 */
nrfjprogdll_err_t NRFJPROG_read_connected_emu_snr_inst(nrfjprog_inst_t instance, uint32_t * serial_number);
nrfjprogdll_err_t NRFJPROG_read_connected_emu_snr(uint32_t * serial_number);


/**
 * @brief   Attempts to reset the connected J-Link OB.
 *
 * @details Resets and reconnects to the J-Link OB.
 *          This operation is only available in debug probes of type J-Link OB-SAM3U128-V2-NordicSemi.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection,
 *          see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the PC will still be connected to the emulator.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() has not been called.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected debug probe does not support the ResetJLink command.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error, check log for details.
 * @retval  EMULATOR_NOT_CONNECTED              The emulator did not successfully reenumerate within 10s after the reset.
 */
nrfjprogdll_err_t NRFJPROG_reset_connected_emu_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_reset_connected_emu(void);


/**
 * @brief   Replaces the firmware on the connected J-Link debug probe.
 *
 * @details Replaces the firmware on the selected debug probe.
 *          The debug probe firmware is replaced with the fw version that shipped with the J-Link DLL selected in open_dll()
 *          even if a newer version is already present.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection,
 *          see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the debug probe will have been reset.
 * @post    After the execution of this function, the PC will still be connected to the emulator.
 * 
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error, check log for details.
 * @retval  EMULATOR_NOT_CONNECTED              The emulator did not successfully reenumerate within 10s after the reset.
 */
nrfjprogdll_err_t NRFJPROG_replace_connected_emu_fw_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_replace_connected_emu_fw(void);


/**
 * @brief   Reads the firmware identification string of the emulator connected to.
 *
 * @details This function reads the firmware identification string of the emulator connected to into the
 *          given buffer. The function will read a maximum of buffer_size-1 characters into the buffer, and 0-terminate it. 
 *          Any excess characters are not read.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   buffer                              Pointer to buffer to contain the firmware string.
 * @param   buffer_size                         Size of the buffer. The user is responsible for ensuring a big enough buffer. A 255 byte buffer is suggested.
 *                                              Maximum buffer_size value is INT_MAX (2147483647).
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   NRFJPROG_open_dll() function has not been called.
 *                                              NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() has not been called.
 * @retval  INVALID_PARAMETER                   The character buffer pointer is a NULL-pointer.
 */
nrfjprogdll_err_t NRFJPROG_read_connected_emu_fwstr_inst(nrfjprog_inst_t instance, char * buffer, uint32_t buffer_size);
nrfjprogdll_err_t NRFJPROG_read_connected_emu_fwstr(char * buffer, uint32_t buffer_size);


/**
 * @brief   Disconnects from an emulator.
 *
 * @details This function disconnects from a connected emulator. This also disconnects from a connected device if connected. Will
 *          not fail if we have never connected to an emulator. After the execution of this function, the device CPU will not change
 *          its state from running or halted.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @post    After the execution of this function, the device will not be in debug interface mode. To enter debug interface mode, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the PC will be disconnected from an emulator. To connect to an emulator, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @post    After the execution of this function, the emulator will be disconnected from the device. To connect to the device, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the trace and debug resources will be switched off. To switch on the debug resources, see NRFJPROG_connect_to_device() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_disconnect_from_emu_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_disconnect_from_emu(void);

/**
 * @brief   Selects a new family
 *
 * @details This function switches to the dll api that belongs to the family passed in. J-Link state is preserved.
 *          Its primary use is for switching from UNKNOWN family to a known family, but the function also supports other
 *          switches. No family-specific cleanup is performed before switching family. There's a risk that the device will be
 *          left in an unknown state when other transitions are done than from UNKNOWN_FAMILY.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @post    After the execution of this function, the api will call the functions belonging to the selected family dll.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   family                              New family to interact with
 */
nrfjprogdll_err_t NRFJPROG_select_family_inst(nrfjprog_inst_t instance, device_family_t family);
nrfjprogdll_err_t NRFJPROG_select_family(device_family_t family);


/**
 * @brief   Operation not available for devices of NRF52 family.
 *
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 */
nrfjprogdll_err_t NRFJPROG_is_coprocessor_enabled_inst(nrfjprog_inst_t instance, coprocessor_t coprocessor, bool * is_coprocessor_enabled);
nrfjprogdll_err_t NRFJPROG_is_coprocessor_enabled(coprocessor_t coprocessor, bool * is_coprocessor_enabled);

/**
 * @brief   Operation not available for devices of NRF52 family.
 *
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 */
nrfjprogdll_err_t NRFJPROG_enable_coprocessor_inst(nrfjprog_inst_t instance, coprocessor_t coprocessor);
nrfjprogdll_err_t NRFJPROG_enable_coprocessor(coprocessor_t coprocessor);


/**
 * @brief   Operation not available for devices of NRF52 family.
 *
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 */
nrfjprogdll_err_t NRFJPROG_disable_coprocessor_inst(nrfjprog_inst_t instance, coprocessor_t coprocessor);
nrfjprogdll_err_t NRFJPROG_disable_coprocessor(coprocessor_t coprocessor);


/**
 * @brief   Operation not available for devices of NRF52 family.
 *
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 */
nrfjprogdll_err_t NRFJPROG_select_coprocessor_inst(nrfjprog_inst_t instance, coprocessor_t coprocessor);
nrfjprogdll_err_t NRFJPROG_select_coprocessor(coprocessor_t coprocessor);


/**
 * @brief   Recovers the device.
 *
 * @details This operation attempts to recover the device and leave it as it was when it left Nordic factory. It will attempt to
 *          connect, erase all user available flash, halt and eliminate any protection. Note that this operation may take up to 30 s
 *          if the device was readback protected. Note as well that this function only affects flash and CPU, but does not reset or 
 *          stop any peripheral or oscillator source. The operation will therefore leave the watchdog still operational.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    After the execution of this function, all device RAM will be powered. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 * @post    After the execution of this function, the device code and UICR flash will be erased.
 * @post    After the execution of this function, if the device was readback protected, the device RAM will be erased.
 * @post    After the execution of this function, if the device was readback protected, the device will no longer be readback protected.
 * @post    After the execution of this function, the POWER.RESETREAS register will be cleared.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  RECOVER_FAILED                      Recover failed for any reason, including that the wrong family was used in the NRFJPROG_open_dll() function.
 */
nrfjprogdll_err_t NRFJPROG_recover_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_recover(void);


/**
 * @brief   Checks if the connected emulator has an established connection with an nRF device.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   is_emu_connected_to_device          Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The is_emu_connected_to_device pointer is NULL.
 */
nrfjprogdll_err_t NRFJPROG_is_connected_to_device_inst(nrfjprog_inst_t instance, bool * is_emu_connected_to_device);
nrfjprogdll_err_t NRFJPROG_is_connected_to_device(bool * is_emu_connected_to_device);


/**
 * @brief   Connects to the nRF device.
 *
 * @details This function connects the emulator to the nRF device, powering the debug resources and entering debug interface mode.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 * @pre     Before the execution of this function, the emulator must not be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the trace and debug resources will be switched on. To switch off the debug resources, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_connect_to_device() function has already been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_connect_to_device_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_connect_to_device(void);


/**
 * @brief   Disconnects from the nRF device.
 *
 * @details Disconnects the emulator from the nRF device, by executing the exit sequence. If the emulator was not connected to the device, no operation is performed.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the device will not be in debug interface mode. To enter debug interface mode, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the emulator will be disconnected from the device. To connect to the device, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the trace and debug resources will be switched off. To switch on the debug resources, see NRFJPROG_connect_to_device() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  EMULATOR_NOT_CONNECTED              The emulator with which a connection was established is no longer connected to the PC.
 */
nrfjprogdll_err_t NRFJPROG_disconnect_from_device_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_disconnect_from_device(void);


/**
 * @brief   Protects the device against read or debug.
 *
 * @details Protects the device against read or debug by writing into UICR.RBPCONF and resetting. The function will reset
 *          with a debug reset after execution. The only protection level allowed for nRF52 devices is "ALL". Only devices
 *          with CTRL-AP can readback protect, so nRF52 Engineering sample A devices are excluded.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the device debug access port will be disconnected. To reconnect the access port, see function NRFJPROG_recover().
 * @post    After the execution of this function, the device CPU will be running. It is not possible to halt the device.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   desired_protection                  Desired protection level of readback protection.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The desired_protection is NONE, REGION_0 or BOTH.
 *                                              The desired_protection cannot be encoded in readback_protection_status_t.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_readback_protect_inst(nrfjprog_inst_t instance, readback_protection_status_t desired_protection);
nrfjprogdll_err_t NRFJPROG_readback_protect(readback_protection_status_t desired_protection);


/**
 * @brief   Returns the status of the readback protection.
 *
 * @details Returns the status of the readback protection.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   status                              Pointer for storing of readback status.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The status pointer is NULL.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_readback_status_inst(nrfjprog_inst_t instance, readback_protection_status_t * status);
nrfjprogdll_err_t NRFJPROG_readback_status(readback_protection_status_t * status);


/**
 * @brief   Returns the status of the erase protection.
 *
 * @details nRF52 series devices do not implement erase protection.
 *          This function always sets status to false, and returns an error.
 *
 * @param   status                              Pointer for storing of eraseprotect status.
 *
 * @retval  INVALID_DEVICE_FOR_OPERATION        This family does not support ERASEPROTECT.
 * @retval  INVALID_PARAMETER                   The status pointer is NULL.
 */
nrfjprogdll_err_t NRFJPROG_is_eraseprotect_enabled_inst(nrfjprog_inst_t instance, bool * status);
nrfjprogdll_err_t NRFJPROG_is_eraseprotect_enabled(bool * status);

/**
 * @brief   Enable erase protection
 *
 * @details nRF52 series devices do not implement erase protection.
 *          This function always returns an error.
 *
 * @retval  INVALID_DEVICE_FOR_OPERATION        This family does not support ERASEPROTECT.
 */
nrfjprogdll_err_t NRFJPROG_enable_eraseprotect_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_enable_eraseprotect();


/**
 * @brief   Since region 0 cannot be configured in NRF52 devices, returns 0 size and NO_REGION_0 source.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   size                                Pointer for storing of region 0 protection size.
 * @param   source                              Pointer for storing of region 0 protection source.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The size pointer is NULL.
 *                                              The source pointer is NULL.
 */
nrfjprogdll_err_t NRFJPROG_read_region_0_size_and_source_inst(nrfjprog_inst_t instance, uint32_t * size, region_0_source_t * source);
nrfjprogdll_err_t NRFJPROG_read_region_0_size_and_source(uint32_t * size, region_0_source_t * source);


/**
 * @brief   Executes a soft reset using the CTRL-AP.
 *
 * @details Executes a soft reset by the use of the CTRL-AP RESET register.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the emulator will be disconnected from the device. To connect to the device, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the device CPU will be running. To halt the device CPU, see NRFJPROG_halt() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 */
nrfjprogdll_err_t NRFJPROG_debug_reset_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_debug_reset(void);


/**
 * @brief   Executes a system reset request.
 *
 * @details Executes a cortex-M standard sys reset request by the use of SCB.AIRCR register.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_sys_reset_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_sys_reset(void);


/**
 * @brief   Executes a pin reset.
 *
 * @details Executes a pin reset by lowering to GND the nReset pin in the SWD connector for 20 ms.
 *          In order for the function execution to have the desired effect, the pin reset must be enabled in UICR.PSELRESET[] registers.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @during  During the execution of this function, the emulator mode is changed to JTAG. If the execution fails, the emulator might be left in JTAG mode. If the execution fails, execute NRFJPROG_close_dll() and start anew.
 *
 * @post    After the execution of this function, the device will not be in debug interface mode. To enter debug interface mode, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the emulator will be disconnected from the device. To connect to the device, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the trace and debug resources will be switched off. To switch on the debug resources, see NRFJPROG_connect_to_device() function.
 * @post    After the execution of this function, the device CPU will be running. To halt the device CPU, see NRFJPROG_halt() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_pin_reset_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_pin_reset(void);


/**
 * @brief   Disables BPROT.
 *
 * @details Disables BPROT by writing into register BPROT.DISABLEINDEBUG or ACL by writing into register ACL.ACLINDEBUG where appropriate depending on device.
 *          The method then halts the target device and performs a system reset to clear any block protection settings.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    After the execution of this function, if UICR->APPROTECT was written, readback protection may be enabled. If so, the function will fail with NOT_AVAILABLE_BECAUSE_PROTECTION.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection, or the operation triggered readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 */
nrfjprogdll_err_t NRFJPROG_disable_bprot_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_disable_bprot(void);


/**
 * @brief   Detects if memory block protection is enabled.
 *
 * @details Checks target device BPROT or ACL configuration, and returns bprot_enabled = true if the address range address_start to address_start + length overlaps a region with write restrictions.
 *          If the range address_start -> address_start + length falls outside code flash, BPROT and ACL protection is assumed disabled for the part that falls outside flash memory.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   bprot_enabled                       Pointer to a boolean value to store the query result in.
 * @param   address_start                       Query address range start.
 * @param   length                              Query address range length.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The bprot_enabled parameter is a NULL pointer.
 *                                              The length parameter is 0.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 */
nrfjprogdll_err_t NRFJPROG_is_bprot_enabled_inst(nrfjprog_inst_t instance, bool * bprot_enabled, uint32_t address_start, uint32_t length);
nrfjprogdll_err_t NRFJPROG_is_bprot_enabled(bool * bprot_enabled, uint32_t address_start, uint32_t length);


/**
 * @brief   Erases all code and UICR flash.
 *
 * @details Erases all code and UICR flash regions. Note that erase_all() will not disable BPROT or ACL.
 *          See NRFJPROG_disable_bprot() funtion to disable BPROT and ACL while in debug interface mode.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_erase_all_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_erase_all(void);


/**
 * @brief   Erases a page of code flash.
 *
 * @details Erases a page of code flash containing the addr. Note that NRFJPROG_erase_page() will not disable BPROT if enabled while in debug
 *          interface mode. See NRFJPROG_disable_bprot() funtion to disable BPROT while in debug interface mode.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address of the code flash page to erase.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  NOT_AVAILABLE_BECAUSE_BPROT         The page is not erasable because it is configured as write protected by the BPROT or ACL modules. Call NRFJPROG_disable_bprot() to allow the page to be erased.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_erase_page_inst(nrfjprog_inst_t instance, uint32_t addr);
nrfjprogdll_err_t NRFJPROG_erase_page(uint32_t addr);


/**
 * @brief   Erases UICR info page.
 *
 * @details Erases UICR info page.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_erase_uicr_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_erase_uicr(void);


/**
 * @brief   Writes one uint32_t data into the given address.
 *
 * @details Writes one uint32_t data to the given addr without verifying that the address is accessible or even exists.
 *          If nvmc_control is true, it will control the NVMC in order to write into flash. Writes need to be 32-bit aligned.
 *          Note that if the target address is in unpowered RAM, the operation will fail.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to write to.
 * @param   data                                Value to write.
 * @param   nvmc_control                        If the target address needs NVMC control.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The addr parameter is not 32-bit aligned.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 *                                              The address to write is in unpowered RAM.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_write_u32_inst(nrfjprog_inst_t instance, uint32_t addr, uint32_t data, bool nvmc_control);
nrfjprogdll_err_t NRFJPROG_write_u32(uint32_t addr, uint32_t data, bool nvmc_control);


/**
 * @brief   Reads one uint32_t from the given address.
 *
 * @details Reads one uint32_t data from the given addr without verifying that the address is accessible or even exists.
 *          Reads need to be 32-bit aligned. Note that if the target address is in unpowered RAM, the operation will fail.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to read from.
 * @param   data                                Pointer of the location to store the value.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The addr parameter is not 32-bit aligned.
 *                                              The data parameter is null.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 *                                              The address to read is in unpowered RAM.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_read_u32_inst(nrfjprog_inst_t instance, uint32_t addr, uint32_t * data);
nrfjprogdll_err_t NRFJPROG_read_u32(uint32_t addr, uint32_t * data);


/**
 * @brief   Writes data from the array into the device starting at the given address.
 *
 * @details Writes data_len bytes from the data array into the device starting at the given addr without verifying that the address is accessible
 *          or even exists. If nvmc_control is true, it will control the NVMC in order to write into flash. Writing addr does not need to be 32-bit 
 *          aligned. Writing length data_len does not need to be a multiple of four, but please note that the number of writes to a flash word between 
 *          erases is limited (see nWRITE parameter in the Product Specification for your device) so data_len less than four bytes should be avoided
 *          if possible. Note that if the target address is in unpowered RAM, the operation will fail.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Start address of the region to write to.
 * @param   data                                Pointer to an array with the data to write.
 * @param   data_len                            Length of the data array.
 * @param   nvmc_control                        If the target address needs NVMC control.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The data_len parameter is 0.
 *                                              The data parameter is NULL.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 *                                              The address to write is in unpowered RAM.
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_write_inst(nrfjprog_inst_t instance, uint32_t addr, const uint8_t * data, uint32_t data_len, bool nvmc_control);
nrfjprogdll_err_t NRFJPROG_write(uint32_t addr, const uint8_t * data, uint32_t data_len, bool nvmc_control);


/**
 * @brief   Reads data_len bytes from the device starting at the given address.
 *
 * @details Reads data_len bytes from the device starting at the given addr without verifying that the addresses are accessible or even
 *          exist. Note that if the target address is in unpowered RAM, the operation will fail.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to read from.
 * @param   data                                Pointer of the location to store the value.
 * @param   data_len                            Number of bytes to read.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_len parameter is 0.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 *                                              The address to read is in unpowered RAM.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_read_inst(nrfjprog_inst_t instance, uint32_t addr, uint8_t * data, uint32_t data_len);
nrfjprogdll_err_t NRFJPROG_read(uint32_t addr, uint8_t * data, uint32_t data_len);


/**
 * @brief   Checks if the device CPU is halted.
 *
 * @details Checks if the device CPU is halted.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   is_device_halted                    Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  INVALID_PARAMETER                   The is_device_halted pointer is NULL.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_is_halted_inst(nrfjprog_inst_t instance, bool * is_device_halted);
nrfjprogdll_err_t NRFJPROG_is_halted(bool * is_device_halted);


/**
 * @brief   Halts the device CPU.
 *
 * @details Halts the device CPU.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_halt_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_halt(void);


/**
 * @brief   Starts the device CPU with the given pc and sp.
 *
 * @details Starts the device CPU with the given pc and sp.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @during  During the execution of this function, the device CPU is halted. If the function execution fails, the device CPU might be left halted.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be running. To halt the device CPU, see NRFJPROG_halt() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   pc                                  Program Counter to start running from.
 * @param   sp                                  Stack Pointer to use when running.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_run_inst(nrfjprog_inst_t instance, uint32_t pc, uint32_t sp);
nrfjprogdll_err_t NRFJPROG_run(uint32_t pc, uint32_t sp);


/**
 * @brief   Starts the device CPU.
 *
 * @details Starts the device CPU.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be running. To halt the device CPU, see NRFJPROG_halt() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_go_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_go(void);


/**
 * @brief   Runs the device CPU for one instruction.
 *
 * @details Runs the device CPU for one instruction and halts.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 * @pre     Before the execution of this function, the device CPU must be halted. To halt the device CPU, see NRFJPROG_halt() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The device is not halted.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_step_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_step(void);


/**
 * @brief   Reads the number of RAM sections in the device.
 *
 * @details Reads the number of RAM sections in device and returns the result in input pointer.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   ram_sections_count                  Pointer of the location to store the number of RAM section in the device.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The ram_sections_count pointer is NULL.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_read_ram_sections_count_inst(nrfjprog_inst_t instance, uint32_t * ram_sections_count);
nrfjprogdll_err_t NRFJPROG_read_ram_sections_count(uint32_t * ram_sections_count);


/**
 * @brief   Reads the size in bytes of the RAM sections in device.
 *
 * @details Reads the size of the RAM sections in device and returns the result in the input array in bytes. If input array size is larger
 *          than the number of RAM sections, the rest of the array will be left untouched.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   ram_sections_size                   Array to store the results.
 * @param   ram_sections_size_len               Size of ram_sections_size array.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The ram_sections_size pointer is NULL.
 *                                              The ram_sections_size_len is less than the number of RAM sections in device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_read_ram_sections_size_inst(nrfjprog_inst_t instance, uint32_t * ram_sections_size, uint32_t ram_sections_size_len);
nrfjprogdll_err_t NRFJPROG_read_ram_sections_size(uint32_t * ram_sections_size, uint32_t ram_sections_size_len);


/**
 * @brief   Reads the RAM sections power status.
 *
 * @details Reads the RAM sections power status and returns the result, on or off for each section, in ram_sections_power_status array.
 *          If the device is an NRF52832 ENGA, please note that the RAM sections power status mapping is wrong, see 
 *          Errata-65 from the errata document for nRF52832 devices.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   ram_sections_power_status           Array to store the results.
 * @param   ram_sections_power_status_len       Size of ram_sections_power_status array.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The ram_sections_power_status pointer is NULL.
 *                                              The ram_sections_power_status_len is less than the number of RAM sections in device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_read_ram_sections_power_status_inst(nrfjprog_inst_t instance, ram_section_power_status_t * ram_sections_power_status, uint32_t ram_sections_power_status_len);
nrfjprogdll_err_t NRFJPROG_read_ram_sections_power_status(ram_section_power_status_t * ram_sections_power_status, uint32_t ram_sections_power_status_len);


/**
 * @brief   Powers up all RAM sections of the device.
 *
 * @details Powers up all the RAM of the device in ON state. Will not affect the RAM retention.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_power_ram_all_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_power_ram_all(void);


/**
 * @brief   Powers down a RAM section of the device.
 *
 * @details Powers down a RAM section of the device in ON state. Will not affect the RAM retention. If the device is an
 *          NRF52832 ENGA, please note that the RAM sections power status mapping is wrong, see Errata-65 from the errata
 *          document for nRF52 devices. The use of this function is therefore discouraged.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   section_index                       Section of RAM to power down.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The section section_index does not exist in the device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_unpower_ram_section_inst(nrfjprog_inst_t instance, uint32_t section_index);
nrfjprogdll_err_t NRFJPROG_unpower_ram_section(uint32_t section_index);


/**
 * @brief   Read memory descriptors.
 *
 * @details The 'memories' array is populated with descriptions of each of the memories that are available for the currently selected coprocessor, and the number of memories written is stored in the 'num_memories_available' parameter.
 *              If the function is called with a NULL array and zero length, the total number of available memories is stored in 'num_memories_available'. See header DllCommonDefinitions.h for type definitions.
 *              If the function returns an UNKNOWN_DEVICE error, the device version was not recognized and default values based on family have been generated.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   memories                            Array for storing the memory descriptors. Can be NULL of 'memories_len' is zero.
 * @param   memories_len                        Length of the 'memories' array, and the max number of memory descriptors that will be written.
 * @param   num_memories_available              Pointer for storing the number of memory descriptions available. Can be NULL.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   'memories' parameter is NULL while 'memories_len' is non-zero.
 *                                              'num_memories_available' is NULL.
 * @retval  INVALID_OPERATION                   The NRFJPROG_open_dll() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  TIME_OUT                            Timed out waiting for the device debug systems to power up.
 * @retval  UNKNOWN_DEVICE                      Device version was unrecognized.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    Memory descriptions unavailable due to device readback protection.
 */
nrfjprogdll_err_t NRFJPROG_read_memory_descriptors(memory_description_t memories[], uint32_t memories_len, uint32_t * num_memories_available);
nrfjprogdll_err_t NRFJPROG_read_memory_descriptors_inst(nrfjprog_inst_t instance, memory_description_t memories[], uint32_t memories_len, uint32_t * num_memories_available);


/**
 * @brief   Read the page sizes of a memory.
 *
 * @details Read the page sizes of the memory referenced by the provided memory_description struct, and store the number of page repetitions written in 'num_page_reps_available'.
 *              If the function is called with a NULL array and zero length, the total number of available page repetitions is stored in 'num_page_reps_available'.
 *              See header DllCommonDefinitions.h for type definitions.
 *
 *              Each element of the 'page_reps' array contains a page size, and the number of times the page is repeated. For instance, if a memory has homogenous page size,
 *              one element will be written. The address of a page can be found by adding the memory start address to the sum of pages before it. Pages are ordered
 *              the way they appear in the 'page_reps' array, where the first page repetitions element describes the first pages of a memory.
 *
 *              Note that the memories returned by NRFJPROG_read_memory_descriptors() changes based on selected coprocessor and read device version, potentially rendering the memory
 *              reference 'memory_description' invalid.
 *
 * @pre     A memory device struct has been obtained with the NRFJPROG_read_memory_descriptors() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   memory_description                  Pointer to a memory description struct that has been obtained with the NRFJPROG_read_memory_descriptors() function.
 * @param   page_reps                           Array for storing page repetitions. Can be NULL if 'page_reps_len' is zero.
 * @param   page_reps_len                       Length of the 'page_reps' array, and the max number of page repetitions that will be written.
 * @param   num_page_reps_available             Pointer for storing the number of page repetitions available.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   'memory_description' is NULL.
 *                                              'num_page_reps_available' is NULL.
 *                                              'page_reps' parameter is NULL while 'page_reps_len' is non-zero.
 *                                              The memory referenced by 'memory_description' was not found.
 */
nrfjprogdll_err_t NRFJPROG_read_page_sizes(memory_description_t * memory_description, page_repetitions_t page_reps[], uint32_t page_reps_len, uint32_t * num_page_reps_available);
nrfjprogdll_err_t NRFJPROG_read_page_sizes_inst(nrfjprog_inst_t instance, memory_description_t * memory_description, page_repetitions_t page_reps[], uint32_t page_reps_len, uint32_t * num_page_reps_available);


/**
 * @brief   Reads a CPU register.
 *
 * @details Reads a CPU register. Valid registers are R0-R15, XPSR, MSP and PSP.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   register_name                       Register name to read. See cpu_registers_t definition for valid values.
 * @param   register_value                      Pointer of the location to store the read register.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The register_data parameter is null.
                                                The register_name parameter is not a valid register.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_read_cpu_register_inst(nrfjprog_inst_t instance, cpu_registers_t register_name, uint32_t * register_value);
nrfjprogdll_err_t NRFJPROG_read_cpu_register(cpu_registers_t register_name, uint32_t * register_value);


/**
 * @brief   Writes a CPU register.
 *
 * @details Writes a CPU register. Valid registers are R0-R15, XPSR, MSP and PSP.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   register_name                       CPU register to write. See cpu_registers_t definition for valid values.
 * @param   register_value                      Value to write into the CPU register.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The register_name parameter is not a valid register.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 */
nrfjprogdll_err_t NRFJPROG_write_cpu_register_inst(nrfjprog_inst_t instance, cpu_registers_t register_name, uint32_t register_value);
nrfjprogdll_err_t NRFJPROG_write_cpu_register(cpu_registers_t register_name, uint32_t register_value);


/**
 * @brief   Reads the version of the device connected to the emulator.
 *
 * @details Reads the version of the device connected to the emulator. If the device is not an NRF52 device version parameter will be set to UNKNOWN and WRONG_FAMILY_FOR_DEVICE error returned.
 *          If the device in use is a newer revision of a known NRF52 device, but this revision is not known at compile time of this dll, the appropriate device _FUTURE will be returned instead.
 *          See DllCommonDefinitions.h header file for the possible values of x. Consider updating to a newer version of tools.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   version                             Pointer of the location to store the device type.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The version parameter is null.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 */
nrfjprogdll_err_t NRFJPROG_read_device_version_inst(nrfjprog_inst_t instance, device_version_t * version);
nrfjprogdll_err_t NRFJPROG_read_device_version(device_version_t * version);


/**
 * @brief   Reads the version, name, memory and revision descriptors of the device connected to the emulator.
 *
 * @details Reads the version, name, memory and revision descriptors of the device connected to the emulator. If the device is not an NRF52 device version, name, memory and revision parameters will be set to UNKNOWN, UNKNOWN_NAME, UNKNOWN_MEM and UNKNOWN_REV and WRONG_FAMILY_FOR_DEVICE error is returned.
 *          If the device in use is a newer revision of a known NRF52 device, but this revision is not known at compile time of this dll, device revision will be set to FUTURE.
 *          See DllCommonDefinitions.h header file for the possible values of x. Consider updating to a newer version of tools.
 *          If a NULL-pointer is passed as an argument, that parameter is not updated.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   version                             Pointer of the location to store the device version.
 * @param   name                                Pointer of the location to store the device name.
 * @param   memory                              Pointer of the location to store the device memory.
 * @param   revision                            Pointer of the location to store the device revision.
 * 
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 */
nrfjprogdll_err_t NRFJPROG_read_device_info_inst(nrfjprog_inst_t instance, device_version_t * version, device_name_t * name, device_memory_t * memory, device_revision_t * revision);
nrfjprogdll_err_t NRFJPROG_read_device_info(device_version_t * version, device_name_t * name, device_memory_t * memory, device_revision_t * revision);


/**
 * @brief   Operation not available for this dll.
 *
 * @retval  INVALID_OPERATION                   This function cannot be executed in this dll.
 */
nrfjprogdll_err_t NRFJPROG_read_device_family_inst(nrfjprog_inst_t instance, device_family_t * family);
nrfjprogdll_err_t NRFJPROG_read_device_family(device_family_t * family);


/**
 * @brief   Reads a debug port register.
 *
 * @details Reads into data pointer a debug port register.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, the emulator must not be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   reg_addr                            Register address to read, either in debug port or access port.
 * @param   data                                Pointer of the location to store the value read.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_connect_to_device() function has already been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The register address is not 32-bit aligned.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_read_debug_port_register_inst(nrfjprog_inst_t instance, uint8_t reg_addr, uint32_t * data);
nrfjprogdll_err_t NRFJPROG_read_debug_port_register(uint8_t reg_addr, uint32_t * data);


/**
 * @brief   Writes a debug port register.
 *
 * @details Writes data parameter into a debug port register.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, the emulator must not be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   reg_addr                            Register address to write, either in debug port or access port.
 * @param   data                                Data to write into the register.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_connect_to_device() function has already been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The register address is not 32-bit aligned.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_write_debug_port_register_inst(nrfjprog_inst_t instance, uint8_t reg_addr, uint32_t data);
nrfjprogdll_err_t NRFJPROG_write_debug_port_register(uint8_t reg_addr, uint32_t data);


/**
 * @brief   Reads a debugger access port register.
 *
 * @details Reads into data pointer a debugger access port register.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, the emulator must not be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   ap_index                            Access port index for read if ap_access.
 * @param   reg_addr                            Register address to read, either in debug port or access port.
 * @param   data                                Pointer of the location to store the value read.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_connect_to_device() function has already been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The register address is not 32-bit aligned.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_read_access_port_register_inst(nrfjprog_inst_t instance, uint8_t ap_index, uint8_t reg_addr, uint32_t * data);
nrfjprogdll_err_t NRFJPROG_read_access_port_register(uint8_t ap_index, uint8_t reg_addr, uint32_t * data);


/**
 * @brief   Writes a debugger access port register.
 *
 * @details Writes data parameter into a debugger access port register.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, the emulator must not be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   ap_index                            Access port index for write if ap_access.
 * @param   reg_addr                            Register address to write, either in debug port or access port.
 * @param   data                                Data to write into the register.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_connect_to_device() function has already been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The register address is not 32-bit aligned.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_write_access_port_register_inst(nrfjprog_inst_t instance, uint8_t ap_index, uint8_t reg_addr, uint32_t data);
nrfjprogdll_err_t NRFJPROG_write_access_port_register(uint8_t ap_index, uint8_t reg_addr, uint32_t data);


/**
 * @brief   Checks if the RTT is started.
 *
 * @details Checks if the NRFJPROG_rtt_start() function has been called since the last call to NRFJPROG_rtt_stop() or since the loading of this dll.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   started                             Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The started parameter is null.
 */
nrfjprogdll_err_t NRFJPROG_is_rtt_started_inst(nrfjprog_inst_t instance, bool * started);
nrfjprogdll_err_t NRFJPROG_is_rtt_started(bool * started);


/**
 * @brief   Indicates to the dll the location of the RTT control block in the device memory.
 *
 * @details Indicates to the dll the location of the RTT control block in the device memory, to accelerate the discovery of the RTT control block.
 *          If the RTT control block is not located at the address given, NRFJPROG_rtt_start() will never locate the RTT control block.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the RTT must not be started. To stop the RTT, see NRFJPROG_rtt_stop() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   address                             RTT control block address in device memory.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_rtt_start() function has been called.
 */
nrfjprogdll_err_t NRFJPROG_rtt_set_control_block_address_inst(nrfjprog_inst_t instance, uint32_t address);
nrfjprogdll_err_t NRFJPROG_rtt_set_control_block_address(uint32_t address);


/**
 * @brief   Starts RTT.
 *
 * @details Starts RTT, initiating the search for the RTT control block and preparing the DLL for RTT operations. If NRFJPROG_rtt_set_control_block_address() has been called,
 *          the address provided will be used to locate the control block. Since NRFJPROG_rtt_start() function is not blocking, to find out if NRFJPROG_rtt_start() has located
 *          the RTT control block and is ready to receive commands, see NRFJPROG_rtt_is_control_block_found() function. After the execution of NRFJPROG_rtt_start(), please
 *          ensure that NRFJPROG_rtt_stop() function will be executed.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the RTT must not be started. To stop the RTT, see NRFJPROG_rtt_stop() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, the RTT control block must be present in RAM. Normally it is the firmware execution that writes the control block into RAM. To run the firmware, see NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_rtt_start() function has already been called.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_rtt_start_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_rtt_start(void);


/**
 * @brief   Checks if RTT control block has been found.
 *
 * @details Checks if RTT control block has been found. Can be used to make sure RTT is ready to be used before calling NRFJPROG_rtt_read() and NRFJPROG_rtt_write() functions.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the RTT must be started. To start the RTT, see NRFJPROG_rtt_start() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, a connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   is_control_block_found              Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The is_found parameter is null.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_rtt_is_control_block_found_inst(nrfjprog_inst_t instance, bool * is_control_block_found);
nrfjprogdll_err_t NRFJPROG_rtt_is_control_block_found(bool * is_control_block_found);


/**
 * @brief   Stops RTT.
 *
 * @details Stops RTT and clears the RTT Control Buffer.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the RTT must be started. To start the RTT, see NRFJPROG_rtt_start() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, a connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @post    After the execution of this function, the RTT control block in RAM will have been erased. Therefore, before another NRFJPROG_rtt_start() can be issued, the control block must be rewritten to RAM. See NRFJPROG_rtt_start() function for details.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_rtt_stop_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_rtt_stop(void);


/**
 * @brief   Reads from an RTT channel.
 *
 * @details Reads up to data_len characters from the up_channel_index RTT channel.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the RTT must be started. To start the RTT, see NRFJPROG_rtt_start() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, a connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   up_channel_index                    RTT channel index to read from.
 * @param   data                                Pointer of the location to store the value.
 * @param   data_len                            Number of bytes to read.
 * @param   data_read                           Pointer to the location to store the actual number of read characters.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_read parameter is null.
 *                                              There is no channel in the device with the given up_channel_index index.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_rtt_read_inst(nrfjprog_inst_t instance, uint32_t up_channel_index, char * data, uint32_t data_len, uint32_t * data_read);
nrfjprogdll_err_t NRFJPROG_rtt_read(uint32_t up_channel_index, char * data, uint32_t data_len, uint32_t * data_read);


/**
 * @brief   Writes to an RTT channel.
 *
 * @details Writes the given data_len length data string into the down_channel_index RTT channel.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the RTT must be started. To start the RTT, see NRFJPROG_rtt_start() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, a connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   down_channel_index                  RTT channel index to write to.
 * @param   data                                Pointer to a string with the string to write.
 * @param   data_len                            Length of the string.
 * @param   data_written                        Pointer to the location to store the actual number of written characters.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_read parameter is null.
 *                                              There is no channel in the device with the given down_channel_index index.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_rtt_write_inst(nrfjprog_inst_t instance, uint32_t down_channel_index, const char * data, uint32_t data_len, uint32_t * data_written);
nrfjprogdll_err_t NRFJPROG_rtt_write(uint32_t down_channel_index, const char * data, uint32_t data_len, uint32_t * data_written);


/**
 * @brief   Gets the number of RTT channels.
 *
 * @details Gets the number of down and up channels in the device.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the RTT must be started. To start the RTT, see NRFJPROG_rtt_start() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, a connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   down_channel_number                 Pointer to store the number of down channels.
 * @param   up_channel_number                   Pointer to store the number of up channels.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The down_channel_number parameter is null.
 *                                              The up_channel_number parameter is null.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_rtt_read_channel_count_inst(nrfjprog_inst_t instance, uint32_t * down_channel_number, uint32_t * up_channel_number);
nrfjprogdll_err_t NRFJPROG_rtt_read_channel_count(uint32_t * down_channel_number, uint32_t * up_channel_number);


/**
 * @brief   Reads the info from one RTT channel.
 *
 * @details Reads the info from one RTT channel of index channel_index of direction dir. Reads the rtt channel size into channel_size and name into channel_name pointers.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the RTT must be started. To start the RTT, see NRFJPROG_rtt_start() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, a connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   channel_index                       Channel index to read the info from.
 * @param   dir                                 Channel direction of channel to read the info from.
 * @param   channel_name                        Pointer to store the channel name, must be able to store at least 32 characters.
 * @param   channel_name_len                    Size of channel_name storage size.
 * @param   channel_size                        Pointer to store the channel size.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The channel_name parameter is null.
 *                                              The channel_size parameter is null.
 *                                              The channel_size_len parameter is less than 32.
 *                                              The dir parameter is neither UP_DIRECTION or DOWN_DIRECTION defined in rtt_direction_t enum in DllCommonDefinitions.h
 *                                              The channel with index channel_index does not exist in the device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_rtt_read_channel_info_inst(nrfjprog_inst_t instance, uint32_t channel_index, rtt_direction_t dir, char * channel_name, uint32_t channel_name_len, uint32_t * channel_size);
nrfjprogdll_err_t NRFJPROG_rtt_read_channel_info(uint32_t channel_index, rtt_direction_t dir, char * channel_name, uint32_t channel_name_len, uint32_t * channel_size);


/**
 * @brief   Checks if the QSPI peripheral is initialized.
 *
 * @details Checks if the NRFJPROG_qspi_init() function has been called since the last call to NRFJPROG_qspi_uninit() or since the loading of this dll. If the device does not
 *          have a QSPI peripheral, false is returned.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   initialized                         Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The initialized parameter is null.
 */
nrfjprogdll_err_t NRFJPROG_is_qspi_init_inst(nrfjprog_inst_t instance, bool * initialized);
nrfjprogdll_err_t NRFJPROG_is_qspi_init(bool * initialized);


/**
 * @brief   Configures and initializes the QSPI peripheral.
 *
 * @details Initializes the QSPI peripheral, configuring the QSPI peripheral according to init_params. If retain_ram is true, the device RAM used for QSPI operations will be read and stored.
 *          The stored contents will be restored after the execution of NRFJPROG_qspi_uninit() function. NRFJPROG_qspi_init() function does not initialize the external flash memory device in
 *          any way. If the external flash device requires special configuration for the QSPI configuration given by init_params, it is up to the user to do so with the help of NRFJPROG_qspi_custom()
 *          function. Note that for certain external flash memory devices some operation modes are not available, so use the available options with care. 
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the QSPI must not be initialized. To uninitialize the QSPI, see NRFJPROG_qspi_uninit() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    After the execution of this function, all device RAM will be powered. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   retain_ram                          Save the RAM contents of the region used as buffer for the QSPI operations.
 * @param   init_params                         Pointer to the initialization paramters for QSPI.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_qspi_init() function has already been called.
 * @retval  INVALID_PARAMETER                   The qspi_ini_path parameter is null.
 *                                              The supplied .ini file has syntax errors.
 *                                              One of the QSPI parameters failed verification.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_init_inst(nrfjprog_inst_t instance, bool retain_ram, const qspi_init_params_t * init_params);
nrfjprogdll_err_t NRFJPROG_qspi_init(bool retain_ram, const qspi_init_params_t * init_params);


/**
 * @brief   Configures and initializes the QSPI peripheral.
 *
 * @details Initializes the QSPI peripheral, configuring the QSPI peripheral with parameters from the provided ini file. See file QspiDefault.ini for an example.
 *          If retain_ram is true, the device RAM used for QSPI operations will be read and stored.
 *          The stored contents will be restored after the execution of NRFJPROG_qspi_uninit() function. NRFJPROG_qspi_init() function does not initialize the external flash memory device in
 *          any way. If the external flash device requires special configuration for the QSPI configuration given by init_params, it is up to the user to do so with the help of NRFJPROG_qspi_custom()
 *          function. Note that for certain external flash memory devices some operation modes are not available, so use the available options with care. 
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the QSPI must not be initialized. To uninitialize the QSPI, see NRFJPROG_qspi_uninit() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    After the execution of this function, all device RAM will be powered. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   qspi_ini_path                       Path to the .ini containing initialization parameters for the QSPI memory interface.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   Could not find file referenced by qspi_ini_path.
 *                                              Syntax error encountered while parsing QSPI ini file.
 * @retval  OUT_OF_MEMORY                       Ran out of memory while parsing QSPI ini file.
 * @retval  FILE_OPERATION_FAILED               Failed to read ini file.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_init_ini_inst(nrfjprog_inst_t instance, const char * qspi_ini_path);
nrfjprogdll_err_t NRFJPROG_qspi_init_ini(const char * qspi_ini_path);


/**
 * @brief   Initializes the QSPI peripheral.
 *
 * @details Initializes the QSPI peripheral, using the settings from the already configured QSPI peripheral.
 *          If retain_ram has been set, the device RAM used for QSPI operations will be read and stored. The stored contents will be restored after the execution of NRFJPROG_qspi_uninit() function.
 *          NRFJPROG_qspi_start() function does not initialize the external flash memory device in any way.
 *          If the external flash device requires special configuration for the QSPI configuration given by init_params, it is up to the user to do so with the help of NRFJPROG_qspi_custom()
 *          function. Note that for certain external flash memory devices some operation modes are not available, so use the available options with care. 
 *
 *          The QSPI peripheral configuration will remain after uninitializing QSPI, allowing this function to be called directly after.
 *
 * @pre     Before the execution of this function, the QSPI module must be configured. See functions NRFJPROG_qspi_configure() and NRFJPROG_qspi_configure_ini().
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the QSPI must not be initialized. To uninitialize the QSPI, see NRFJPROG_qspi_uninit() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    After the execution of this function, all device RAM will be powered. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_OPERATION                   QSPI has not been configured.
 *                                              QSPI is already initialized.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_start_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_qspi_start();


/**
 * @brief   Configures the QSPI driver.
 *
 * @details Configure the QSPI peripheral according to init_params, without initializing the QSPI peripheral on the device. See file QspiDefault.ini for an example.
 *          After configuration NRFJPROG_qspi_start() can be called to initialize the QSPI peripheral. See function NRFJPROG_qspi_init() for QSPI configuration and initialization in one step.
 *
 *          Note that configuring using qspi_init_params_t does not set qspi memory size which is needed for NRFJPROG_program_file(), NRFJPROG_read_to_file(), NRFJPROG_verify_file() and NRFJPROG_erase_file().
 *          Use NRFJPROG_qspi_set_size() to configure memory size if needed.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the QSPI must not be initialized. To uninitialize the QSPI, see NRFJPROG_qspi_uninit() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   retain_ram                          Save the RAM contents of the region used as buffer for the QSPI operations.
 * @param   init_params                         Pointer to the initialization paramters for QSPI.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              QSPI is initialized.
 * @retval  INVALID_PARAMETER                   The init_params parameter is null.
 *                                              The init_params->read_mode parameter cannot be encoded in a qspi_read_mode_t. 
 *                                              The init_params->write_mode parameter cannot be encoded in a qspi_write_mode_t. 
 *                                              The init_params->frequency parameter cannot be encoded in a qspi_frequency_t. 
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_configure_inst(nrfjprog_inst_t instance, bool retain_ram, const qspi_init_params_t * init_params);
nrfjprogdll_err_t NRFJPROG_qspi_configure(bool retain_ram, const qspi_init_params_t * init_params);


/**
 * @brief   Configures the QSPI peripheral.
 *
 * @details Configure the QSPI peripheral with parameters from the provided ini file. See file QspiDefault.ini for an example.
 *          After configuration NRFJPROG_qspi_start() can be called to initialize the QSPI peripheral. See function NRFJPROG_qspi_init_ini() for QSPI configuration and initialization in one step.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the QSPI must not be initialized. To uninitialize the QSPI, see NRFJPROG_qspi_uninit() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   qspi_ini_path                       Path to the .ini containing initialization parameters for the QSPI memory interface.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              QSPI is initialized.
 * @retval  INVALID_PARAMETER                   The qspi_ini_path parameter is null.
 *                                              The supplied .ini file has syntax errors.
 *                                              One of the QSPI parameters failed verification.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_configure_ini_inst(nrfjprog_inst_t instance, const char * qspi_ini_path);
nrfjprogdll_err_t NRFJPROG_qspi_configure_ini(const char * qspi_ini_path);


/**
 * @brief   Uninitializes the QSPI peripheral.
 *
 * @details Uninitializes the QSPI peripheral. If NRFJPROG_qspi_init() function was called with the retain_ram parameter set as true, the RAM contents at the time of NRFJPROG_qspi_init() call will
 *          be restored.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    After the execution of this function, the device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    After the execution of this function, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    After the execution of this function, if NRFJPROG_qspi_init() function was called with retain_ram parameter set as true, the contents of the RAM used for QSPI operations will be restored.
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_uninit_inst(nrfjprog_inst_t instance);
nrfjprogdll_err_t NRFJPROG_qspi_uninit(void);

/**
 * @brief   Set QSPI RX delay
 *
 * @details Configures the dll to set the QSPI peripheral rx_delay mechanism to account for CLK round trip issues. If this function is not called, a default value will be used.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the QSPI must not be initialized.
 *
 * @post    After the execution of this function, the DLL will be prepared to use rx_delay instead of the built in default value.
 * @post    After the execution of this function, the RAM memory contents might be altered. To restore the contents of the RAM memory, see NRFJPROG_qspi_uninit() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   rx_delay                            Rx delay to set. See the product specification of your device for possible values.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_qspi_init() function has been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_set_rx_delay_inst(nrfjprog_inst_t instance, uint8_t rx_delay);
nrfjprogdll_err_t NRFJPROG_qspi_set_rx_delay(uint8_t rx_delay);


/**
 * @brief   Set QSPI memory size.
 *
 * @details Set QSPI memory size. This size is used when reading the QSPI memory in NRFJPROG_program_file(), NRFJPROG_read_to_file(), NRFJPROG_verify_file(), and NRFJPROG_erase_file() functions.
 *          Size can also be set by initializing QSPI using an ini file. See functions NRFJPROG_qspi_init_ini() and NRFJPROG_qspi_configure_ini().
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   qspi_size                           QSPI memory size in bytes.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 */
nrfjprogdll_err_t NRFJPROG_qspi_set_size_inst(nrfjprog_inst_t instance, uint32_t qspi_size);
nrfjprogdll_err_t NRFJPROG_qspi_set_size(uint32_t qspi_size);

/**
 * @brief   Get QSPI memory size.
 *
 * @details Get QSPI memory size set by NRFJPROG_qspi_set_size() or NRFJPROG_qspi_init_ini()
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   qspi_size                           Pointer to variable where QSPI memory size in bytes should be written.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The qspi_size parameter is null.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The version of the connected device does not support this operation.
 */
nrfjprogdll_err_t NRFJPROG_qspi_get_size_inst(nrfjprog_inst_t instance, uint32_t * qspi_size);
nrfjprogdll_err_t NRFJPROG_qspi_get_size(uint32_t * qspi_size);


/**
 * @brief   Reads from the external QSPI-connected memory.
 *
 * @details Reads data_len characters from addr address of the external QSPI-connected memory. Actual read command address and length sent to the device may be adjusted since the 
 *          QSPI peripheral only accepts 4-byte aligned addresses and multiple of 4 lengths. Any extra bytes read from the external QSPI-connected memory will be discarded.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the QSPI must be initialized. To initialize the QSPI, see NRFJPROG_qspi_init() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, a connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_qspi_init() functions.
 *
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    After the execution of this function, the RAM memory contents might be altered. To restore the contents of the RAM memory, see NRFJPROG_qspi_uninit() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to read from.
 * @param   data                                Pointer of the location to store the read data.
 * @param   data_len                            Number of bytes to read.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_qspi_init() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_len parameter is equal to 0.
 *                                              The addr parameter is greater than 0x00FFFFFFul when QSPI is configured to work with 24-bit addresses.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_read_inst(nrfjprog_inst_t instance, uint32_t addr, uint8_t * data, uint32_t data_len);
nrfjprogdll_err_t NRFJPROG_qspi_read(uint32_t addr, uint8_t * data, uint32_t data_len);


/**
 * @brief   Writes to the external QSPI-connected memory.
 *
 * @details Writes data_len characters to addr address of the external QSPI-connected memory. Actual write command address and length sent to the device may be adjusted since 
 *          the QSPI peripheral only accepts 4-byte aligned addresses and multiple of 4 lengths. Any extra bytes written to the external QSPI-connected memory will be 0xFF
 *          to avoid altering the contents of the flash. Note that NRFJPROG_qspi_write() function does not erase the addresses to be written, so if target addresses are not erased,
 *          the flash contents after NRFJPROG_qspi_write() operation will therefore be undefined.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the QSPI must be initialized. To initialize the QSPI, see NRFJPROG_qspi_init() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, a connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_qspi_init() functions.
 *
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    After the execution of this function, the RAM memory contents might be altered. To restore the contents of the RAM memory, see NRFJPROG_qspi_uninit() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to write to.
 * @param   data                                Pointer of the location of data to write.
 * @param   data_len                            Number of bytes to write.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_qspi_init() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_len parameter is equal to 0.
 *                                              The addr parameter is greater than 0x00FFFFFFul when QSPI is configured to work with 24-bit addresses.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_write_inst(nrfjprog_inst_t instance, uint32_t addr, const uint8_t * data, uint32_t data_len);
nrfjprogdll_err_t NRFJPROG_qspi_write(uint32_t addr, const uint8_t * data, uint32_t data_len);


/**
 * @brief   Erases the external QSPI-connected memory.
 *
 * @details Erases length bytes from addr address of the external QSPI-connected memory. The given addr must be aligned to the desired erase length. Please note that some erase operations
 *          take a long time with certain external flash devices, so this operation might take minutes, specially with a full erase. Note that for certain external flash memory devices some
 *          erase lengths are not supported, please consult your datasheet and use the available options with care.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the QSPI must be initialized. To initialize the QSPI, see NRFJPROG_qspi_init() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, a connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_qspi_init() functions.
 *
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to erase from.
 * @param   length                              Desired erase length.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_qspi_init() function has not been called.
 * @retval  INVALID_PARAMETER                   The length parameter cannot be encoded in a qspi_erase_len_t. 
 *                                              The addr parameter is greater than 0x00FFFFFFul when QSPI is configured to work with 24-bit addresses.
 *                                              The addr parameter is not aligned to erase length.
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_erase_inst(nrfjprog_inst_t instance, uint32_t addr, qspi_erase_len_t length);
nrfjprogdll_err_t NRFJPROG_qspi_erase(uint32_t addr, qspi_erase_len_t length);


/**
 * @brief   Sends a custom instruction to the external QSPI-connected memory.
 *
 * @details Sends the custom instruction with instruction_code code and instruction_length length to the external QSPI-connected memory.
 *          If data_in parameter is NULL, 0x00 will be sent as data in the custom instruction. The data_out parameter can be NULL.
 *          If more than 8 bytes of data is sent, it will start a long frame qspi operation if available in the device. 
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, the QSPI must be initialized. To initialize the QSPI, see NRFJPROG_qspi_init() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, a connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_qspi_init() functions.
 *
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   instruction_code                    Instruction code of the custom instruction.
 * @param   instruction_length                  Length of the custom instruction.
 * @param   data_in                             Pointer to data to send in the custom instruction. Can be NULL if no data is desired to be sent, where 0x00 will be used if needed.
 * @param   data_out                            Pointer to data to write the data received during the custom instruction. Can be NULL if no data is desired.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_qspi_init() function has not been called.
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The instruction_length parameter is larger than 9 for a device that does not support long frame operations.
 * @retval  INVALID_PARAMETER                   The instruction_length parameter is equal to 0.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
nrfjprogdll_err_t NRFJPROG_qspi_custom_inst(nrfjprog_inst_t instance,
                                          uint8_t instruction_code,
                                          uint32_t instruction_length,
                                          const uint8_t * data_in,
                                          uint8_t * data_out);
nrfjprogdll_err_t NRFJPROG_qspi_custom(uint8_t instruction_code,
                                       uint32_t instruction_length,
                                       const uint8_t * data_in,
                                       uint8_t * data_out);


/**
 * @brief   Programs the provided file to the connected device.
 *
 * @details Programs the provided file to the connected device. Supported file formats are ".hex", ".ihex", ".elf", ".bin" and ".zip".
 *          A .zip file containing several images can be passed. Each file within the .zip that has a supported file extension will be programmed individually.
 *          Block protection (BPROT) will be disabled before writing to the device.
 *          No implicit erase or verify operation will be performed. See the NRFJPROG_erase_file() function for erasing the device before programming.
 *          For verifying device memories after programming, see the NRFJPROG_verify_file() function.
 *
 *          If the file contains data in external memory, the QSPI module must be configured before calling this function. See functions NRFJPROG_qspi_init_ini() and NRFJPROG_qspi_configure_ini().
 *          Note that NRFJPROG_qspi_init() and NRFJPROG_qspi_configure() does not set the external memory size and must be used in combination with NRFJPROG_qspi_set_size().
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, if the file has data in external memory the QSPI module must be configured.
 *
 * @post    After the execution of this function the device will be in debug interface mode. To exit debug interface mode see NRFJPROG_reset().
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_reset() and NRFJPROG_run() functions.
 * @post    After the execution of this function, all device RAM will be powered if the file contains data in any part of RAM. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   file_path                           Path to the file to be programmed.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The file path is NULL.
 *                                              The file cannot be opened for reading.
 * @retval  UNKNOWN_MEMORY_ERROR                The file has data at illegal or unknown memory addresses.
 * @retval  FILE_OPERATION_FAILED               Unable to open file.
 *                                              File is empty.
 * @retval  FILE_INVALID_ERROR                  File has overlapping segments of data.
 * @retval  FILE_PARSING_ERROR                  Failed to parse file contents.
 * @retval  FILE_UNKNOWN_FORMAT_ERROR           File has unsupported file ending.
 *                                              File contents did not match file ending format.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available because the device is readback protected.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support an attempted operation.
 * @retval  OUT_OF_MEMORY                       Could not allocate hex file buffer.
 */
nrfjprogdll_err_t NRFJPROG_program_file_inst(nrfjprog_inst_t instance, const char * file_path);
nrfjprogdll_err_t NRFJPROG_program_file(const char * file_path);


/**
 * @brief   Reads the memory of the connected device into the provided file path.
 *
 * @details Reads the memory of the connected device into the provided file path. The areas of the
 *          device to read can be configured by the use of the read_options parameter. See the description of read_options_t for details.
 *          Supported file formats are ".hex", ".ihex", ".elf", ".bin".
 *
 *          If read_options includes external memory, the QSPI module must be configured before calling this function. See functions NRFJPROG_qspi_init_ini() and NRFJPROG_qspi_configure_ini().
 *          Note that NRFJPROG_qspi_init() and NRFJPROG_qspi_configure() does not set the external memory size and must be used in combination with NRFJPROG_qspi_set_size().
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, If read_options includes external memory the QSPI module must be configured.
 *
 * @post    After the execution of this function the device will be in debug interface mode. To exit debug interface mode see NRFJPROG_reset().
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_reset() and NRFJPROG_run() functions.
 * @post    After the execution of this function, all device RAM will be powered readram was specified. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   file_path                           Path to output file.
 * @param   read_options                        Pointer to struct containing read parameters.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The output file path is NULL.
 *                                              The file cannot be overwritten.
 *                                              The read_options pointer is NULL.
 *                                              The file cannot be opened for writing.
 * @retval  UNKNOWN_MEMORY_ERROR                The file has data at illegal or unknown memory addresses.
 * @retval  FILE_OPERATION_FAILED               Unable to open file.
 *                                              Unable to write to file.
 *                                              File is empty.
 * @retval  FILE_INVALID_ERROR                  File has overlapping segments of data.
 * @retval  FILE_PARSING_ERROR                  Failed to parse file contents.
 * @retval  FILE_UNKNOWN_FORMAT_ERROR           File has unsupported file ending.
 *                                              File contents did not match file ending format.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 *                                              The address to read is in unpowered RAM.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available because the device is readback protected.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support an attempted operation.
 * @retval  OUT_OF_MEMORY                       Could not write to the hex file.
 *                                              Could not allocate hex file buffer.
 *                                              Could not extend hex file buffer.
 *                                              Could not save the hex file.
 */
nrfjprogdll_err_t NRFJPROG_read_to_file_inst(nrfjprog_inst_t instance, const char * file_path, read_options_t read_options);
nrfjprogdll_err_t NRFJPROG_read_to_file(const char * file_path, read_options_t read_options);


/**
 * @brief   Compares the contents of the provided file against the contents of the memory of the connected device.
 *
 * @details Compares the contents of the provided file against the contents of the memory of the connected device. The compare method can be configured with the verify_action parameter.
 *          If verify action is set to VERIFY_READ, the memory contents of the device is read using the debugger and directly compared to the contents of the provided file.
 *          In the case of verify action VERIFY_HASH, a program is loaded into the device RAM that can calculate the hash of a memory area using on-board hardware acceleration. This method is usually faster than VERIFY_READ.
 *
 *          Supported file formats are ".hex", ".ihex", ".elf", ".bin" and ".zip". A .zip file containing several images can be passed.
 *          Each file within the .zip that has a supported file extension will be verified individually.
 *
 *          If the file contains data in external memory, the QSPI module must be configured before calling this function. See functions NRFJPROG_qspi_init_ini() and NRFJPROG_qspi_configure_ini().
 *          Note that NRFJPROG_qspi_init() and NRFJPROG_qspi_configure() does not set the external memory size and must be used in combination with NRFJPROG_qspi_set_size().
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, if the file has data in external memory the QSPI module must be configured.
 *
 * @post    After the execution of this function the device will be in debug interface mode. To exit debug interface mode see NRFJPROG_reset().
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_reset() and NRFJPROG_run() functions.
 * @post    After the execution of this function, all device RAM will be powered if the file contains data in any part of RAM. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   file_path                           Path to output file.
 * @param   verify_action                       Type of verify to perform.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  VERIFY_ERROR                        Data in file does not match the contents of the connected device.
 * @retval  INVALID_OPERATION                   Requested verify action is not available.
 *                                              There is data to verify in external memory, but the QSPI module has not been configured.
 * @retval  INVALID_PARAMETER                   The output file path is NULL.
 *                                              The read_options pointer is NULL.
 * @retval  UNKNOWN_MEMORY_ERROR                The file has data at illegal or unknown memory addresses.
 * @retval  FILE_OPERATION_FAILED               Unable to open file.
 *                                              File is empty.
 * @retval  FILE_INVALID_ERROR                  File has overlapping segments of data.
 * @retval  FILE_PARSING_ERROR                  Failed to parse file contents.
 * @retval  FILE_UNKNOWN_FORMAT_ERROR           File has unsupported file ending.
 *                                              File contents did not match file ending format.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available because the device is readback protected.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support an attempted operation.
 * @retval  OUT_OF_MEMORY                       Could not allocate hex file buffer.
 */
nrfjprogdll_err_t NRFJPROG_verify_file_inst(nrfjprog_inst_t instance, const char * file_path, verify_action_t verify_action);
nrfjprogdll_err_t NRFJPROG_verify_file(const char * file_path, verify_action_t verify_action);


/**
 * @brief   Erases the flash of the connected device based on the contents of the provided file.
 *
 * @details Erases the flash of the connected device. If erase mode is set to erase page, only the pages where the provided file contains data will be erased.
 *          The mode of operation can be configured by setting the erase method for the internal device memory and for the external memory.
 *          Block protection (BPROT) will be disabled if needed before erasing the device.
 *
 *          Supported file formats are ".hex", ".ihex", ".elf", ".bin" and ".zip". A .zip file containing several images can be passed.
 *          Each file within the .zip that has a supported file extension will be passed to the erase function individually.
 *
 *          If the file contains data in external memory and qspi_erase_action is set, the QSPI module must be configured before calling this function. See functions NRFJPROG_qspi_init_ini() and NRFJPROG_qspi_configure_ini().
 *          Note that NRFJPROG_qspi_init() and NRFJPROG_qspi_configure() does not set the external memory size and must be used in combination with NRFJPROG_qspi_set_size().
 *
 *          An "erase all" operation can also be triggered using the dedicated NRFJPROG_erase_all() function.
 *
 * @pre     Before the execution of this function, the dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     Before the execution of this function, a connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Before the execution of this function, if the file has data in external memory and qspi_erase_mode is set, the QSPI module must be configured.
 *
 * @post    After the execution of this function the device will be in debug interface mode. To exit debug interface mode see NRFJPROG_reset().
 * @post    After the execution of this function, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_reset() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   file_path                           Path to output file.
 * @param   chip_erase_mode                     Erase mode for internal flash memory
 * @param   qspi_erase_mode                     Erase mode for external memory.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  VERIFY_ERROR                        Data in file does not match the contents of the connected device.
 * @retval  INVALID_OPERATION                   Requested verify action is not available.
 *                                              There is data to verify in external memory, but the QSPI module has not been configured.
 * @retval  INVALID_PARAMETER                   The output file path is NULL.
 *                                              The read_options pointer is NULL.
 * @retval  UNKNOWN_MEMORY_ERROR                The file has data at illegal or unknown memory addresses.
 * @retval  FILE_OPERATION_FAILED               Unable to open file.
 *                                              File is empty.
 * @retval  FILE_INVALID_ERROR                  File has overlapping segments of data.
 * @retval  FILE_PARSING_ERROR                  Failed to parse file contents.
 * @retval  FILE_UNKNOWN_FORMAT_ERROR           File has unsupported file ending.
 *                                              File contents did not match file ending format.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available because the device is readback protected.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support an attempted operation.
 * @retval  OUT_OF_MEMORY                       Could not allocate hex file buffer.
 */
nrfjprogdll_err_t NRFJPROG_erase_file_inst(nrfjprog_inst_t instance, const char * file_path, erase_action_t chip_erase_mode, erase_action_t qspi_erase_mode);
nrfjprogdll_err_t NRFJPROG_erase_file(const char * file_path, erase_action_t chip_erase_mode, erase_action_t qspi_erase_mode);


#if defined(__cplusplus)
}
#endif

#endif  /* JLINKARM_NRF52_NRFJPROGDLL_H */
