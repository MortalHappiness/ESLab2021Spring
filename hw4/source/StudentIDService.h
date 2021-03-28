/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __STUDENT_ID_SERVICE_H__
#define __STUDENT_ID_SERVICE_H__

class StudentIDService {
public:
    const static uint16_t STUDENT_ID_SERVICE_UUID              = 0xB000;
    const static uint16_t STUDENT1_ID_STATE_CHARACTERISTIC_UUID = 0xB001;
    const static uint16_t STUDENT2_ID_STATE_CHARACTERISTIC_UUID = 0xB002;

    StudentIDService(BLE &_ble, char *student1ID, char *student2ID) :
    ble(_ble),
    student1IDState(STUDENT1_ID_STATE_CHARACTERISTIC_UUID, student1ID, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
    student2IDState(STUDENT2_ID_STATE_CHARACTERISTIC_UUID, student2ID, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ)
    {
        GattCharacteristic *charTable[] = {&student1IDState, &student2IDState};
        GattService         studentIDService(StudentIDService::STUDENT_ID_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.gattServer().addService(studentIDService);
    }

private:
    BLE                              &ble;
    ReadOnlyArrayGattCharacteristic<char, 10>  student1IDState;
    ReadOnlyArrayGattCharacteristic<char, 10>  student2IDState;
};

#endif /* #ifndef __STUDENT_ID_SERVICE_H__ */
