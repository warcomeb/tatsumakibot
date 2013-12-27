/******************************************************************************
 * Copyright (C) 2013-2014 Marco Giammarini
 * 
 * Author(s):
 *	Marco Giammarini <m.giammarini@warcomeb.it>
 *	
 * Project: TatsumakiBot
 * Package: -
 * Version: 0.0
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>
 ******************************************************************************/

/**
 * @file Project_Headers/board.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Board utility defines. 
 */

#ifndef __BOARD_H
#define __BOARD_H

//#include "derivative.h"
#include "libohiboard.h" 

#define FW_MAJOR_VERSION         1
#define FW_MINOR_VERSION         0
/**
 *  Time stamp of current firmware version (-5GMT + DST).
 *  http://www.currenttimestamp.com/
 */
#define FW_TIME_VERSION          1378801518 /* TODO: Update */
/** 
 * Macro for board version.
 * 
 * 0: BreadBoard
 * 1: FRDM-KL25Z
 * 2: LOC028
 */
#define PCB_VERSION              0
#define PROJECT_NAME             "TatsumakiBot"
#define PROJECT_COPYRIGTH        "(C) 2013-2014 Marco Giammarini"

#if (PCB_VERSION == 0)

#endif

#endif /* __BOARD_H */
