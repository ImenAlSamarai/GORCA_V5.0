/* ============================================================================

   Copyright (C) 2000, 2009, 2010  Konrad Bernloehr

   This file is part of the eventio/hessio library.

   The eventio/hessio library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library. If not, see <http://www.gnu.org/licenses/>.

============================================================================ */

/** @file fileopen.h
 *  @short Function prototypes for fileopen.c.
 *
 *  @author  Konrad Bernloehr 
 *  @date    @verbatim CVS $Date: 2012/12/03 08:23:07 $ @endverbatim
 *  @version @verbatim CVS $Revision: 1.2 $ @endverbatim
 */

#ifndef FILEOPEN_H__LOADED
#define FILEOPEN_H__LOADED 1

#ifdef __cplusplus
extern "C" {
#endif

/* fileopen.c */
void initpath(const char *default_path);
void listpath (char *buffer, size_t bufsize);
void addpath(const char *name);
FILE *fileopen(const char *fname, const char *mode);
int fileclose(FILE *f);

#ifdef __cplusplus
}
#endif

#endif
