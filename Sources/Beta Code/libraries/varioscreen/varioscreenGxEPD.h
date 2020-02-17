/* varioscreenGxEPD_154 -- 
 *
 * Copyright 2019 Jean-philippe GOI
 * 
 * This file is part of GnuVario-E.
 *
 * ToneHAL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ToneHAL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* 
 *********************************************************************************
 *********************************************************************************
 *                                                                               *
 *                           VarioScreenGxEPD                                    *
 *                                                                               *
 *  version    Date     Description                                              *
 *    1.0    24/09/19                                                            *
 *    1.0.1  28/09/19   Changement de nom des librairies                         *
 *    1.0.2  22/10/19   Ajout écran 2.13''                                       *
 *    1.0.3  03/02/20   Changement nom passage à 290 au lieu de 29							 *
 *    1.0.4  17/02/20   Suppression 2.13 ajouter 2.90P et 2.90L                  *
 *                                                                               *
 *********************************************************************************/
 
 /*
 *********************************************************************************
 *                    conversion image to cpp code                               *
 *                                                                               *
 *      https://javl.github.io/image2cpp/                                        *
 *                                                                               *
 *********************************************************************************/

#ifndef VARIOSCREENGXEPD_H
#define VARIOSCREENGXEPD_H

#include <HardwareConfig.h>

// Ecran 1.54''
#include <varioscreenObjects_154.h>
#include <varioscreenGxEPD_154.h>

// Ecran 2.90'' Paysage
#include <varioscreenGxEPD_290L.h>
#include <varioscreenObjects_290L.h>

// Ecran 2.90'' Portrait
#include <varioscreenGxEPD_290P.h>
#include <varioscreenObjects_290P.h>

#endif