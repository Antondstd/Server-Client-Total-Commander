//
// Created by ad3st on 29.05.2021.
//

#pragma once
#include "server.h"

ListUsers getSocketByID(int id);

void sendUpdate(MYDATA *params);

void sendUpdatetoAll(MYDATA *params);
