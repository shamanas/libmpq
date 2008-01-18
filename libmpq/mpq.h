/*
 *  mpq.h -- some default types and defines.
 *
 *  Copyright (c) 2003-2008 Maik Broemme <mbroemme@plusserver.de>
 *
 *  This source was adepted from the C++ version of StormLib.h and
 *  StormPort.h included in stormlib. The C++ version belongs to
 *  the following authors:
 *
 *  Ladislav Zezula <ladik@zezula.net>
 *  Marko Friedemann <marko.friedemann@bmx-chemnitz.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _MPQ_H
#define _MPQ_H

/* generic includes. */
#include <limits.h>

/* define return value if nothing failed. */
#define LIBMPQ_SUCCESS				0		/* return value for all functions which success. */

/* define archive errors. */
#define LIBMPQ_ARCHIVE_ERROR_OPEN		-1		/* open error on archive file. */
#define LIBMPQ_ARCHIVE_ERROR_CLOSE		-2		/* close error on archive file. */
#define LIBMPQ_ARCHIVE_ERROR_FORMAT		-3		/* archive format errror. */
#define LIBMPQ_ARCHIVE_ERROR_HASH_TABLE		-4		/* hash table in archive is broken. */
#define LIBMPQ_ARCHIVE_ERROR_BLOCK_TABLE	-5		/* block table in archive is broken. */
#define LIBMPQ_ARCHIVE_ERROR_MALLOC		-6		/* memory allocation error for archive. */

/* define the known archive versions. */
#define LIBMPQ_ARCHIVE_VERSION_ONE		0		/* version one used until world of warcraft. */
#define LIBMPQ_ARCHIVE_VERSION_TWO		1		/* version two used from world of warcraft - the burning crusade. */

/* define file errors. */
#define LIBMPQ_FILE_ERROR_OPEN			-1		/* open error on file. */
#define LIBMPQ_FILE_ERROR_CLOSE			-2		/* close error on file. */
#define LIBMPQ_FILE_ERROR_EXIST			-3		/* file does not exist in archive. */
#define LIBMPQ_FILE_ERROR_RANGE			-4		/* filenumber is out of range. */
#define LIBMPQ_FILE_ERROR_MALLOC		-5		/* memory allocation error for file. */
#define LIBMPQ_FILE_ERROR_DECRYPT		-6		/* we don't know the decryption seed. */
#define LIBMPQ_FILE_ERROR_READ			-7		/* read error on file from archive. */
#define LIBMPQ_FILE_ERROR_DECOMPRESS		-8		/* error on decompression. */

/* define generic mpq archive information. */
#define LIBMPQ_MPQ_HEADER_ID			0x1A51504D	/* mpq archive header ('MPQ\x1A') */
#define LIBMPQ_MPQ_HASH_FREE			0xFFFFFFFF	/* hash table entry is empty and has always been empty. */

/* define generic values for returning archive information. */
#define LIBMPQ_ARCHIVE_SIZE			1		/* mpq archive size. */
#define LIBMPQ_ARCHIVE_SIZE_COMPRESSED		2		/* compressed archive size. */
#define LIBMPQ_ARCHIVE_SIZE_UNCOMPRESSED	3		/* uncompressed archive size. */
#define LIBMPQ_ARCHIVE_FILES			4		/* number of files in the mpq archive */
#define LIBMPQ_ARCHIVE_HASH_TABLE_COUNT		5		/* mpq archive hashtable size. */
#define LIBMPQ_ARCHIVE_BLOCK_TABLE_COUNT	6		/* mpq archive blocktable size. */
#define LIBMPQ_ARCHIVE_BLOCK_SIZE		7		/* mpq archive blocksize. */

/* define generic values for returning file information. */
#define LIBMPQ_FILE_SIZE_COMPRESSED		1		/* compressed filesize of the given file in archive. */
#define LIBMPQ_FILE_SIZE_UNCOMPRESSED		2		/* uncompressed filesize of the given file in archive. */
#define LIBMPQ_FILE_TYPE_COMPRESSION		3		/* compression type of the given file in archive.*/

/* define values used by blizzard as flags. */
#define LIBMPQ_FILE_EXISTS			0x80000000	/* set if file exists, reset when the file was deleted. */
#define LIBMPQ_FILE_ENCRYPTED			0x00010000	/* indicates whether file is encrypted. */
#define LIBMPQ_FILE_COMPRESSED			0x0000FF00	/* file is compressed. */
#define LIBMPQ_FILE_COMPRESS_PKWARE		0x00000100	/* compression made by pkware data compression library. */
#define LIBMPQ_FILE_COMPRESS_MULTI		0x00000200	/* multiple compressions. */
#define LIBMPQ_FILE_SINGLE			0x01000000	/* file is stored in one single sector, first seen in world of warcraft. */

/* define true and false, because not all systems have them. */
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/* define min, because not all systems have it. */
#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

/* define max, because not all systems have it. */
#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

/* mpq archive header. */
typedef struct {
	unsigned int	mpq_magic;		/* the 0x1A51504D ('MPQ\x1A') signature. */
	unsigned int	header_size;		/* mpq archive header size. */
	unsigned int	archive_size;		/* size of mpq archive. */
	unsigned short	version;		/* 0000 for starcraft and broodwar. */
	unsigned short	block_size;		/* size of file block is (512 * 2 ^ block size). */
	unsigned int	hash_table_offset;	/* file position of mpq_hash. */
	unsigned int	block_table_offset;	/* file position of mpq_block, each entry has 16 bytes. */
	unsigned int	hash_table_count;	/* number of entries in hash table. */
	unsigned int	block_table_count;	/* number of entries in the block table. */
} __attribute__ ((packed)) mpq_header_s;

/* hash entry, all files in the archive are searched by their hashes. */
typedef struct {
	unsigned int	hash_a;			/* the first two unsigned ints are the encrypted file. */
	unsigned int	hash_b;			/* the first two unsigned ints are the encrypted file. */
	unsigned short	locale;			/* locale information. */
	unsigned short	platform;		/* platform information and zero is default. */
	unsigned int	block_table_index;	/* index to file description block. */
} __attribute__ ((packed)) mpq_hash_s;

/* file description block contains informations about the file. */
typedef struct {
	unsigned int	offset;			/* block file starting position in the archive. */
	unsigned int	compressed_size;	/* compressed file size. */
	unsigned int	uncompressed_size;	/* uncompressed file size. */
	unsigned int	flags;			/* flags. */
} __attribute__ ((packed)) mpq_block_s;

/* file structure used since diablo 1.00 (0x38 bytes). */
typedef struct {
	char		filename[PATH_MAX];	/* filename of the actual file in the archive. */
	int		fd;			/* file handle. */
	unsigned int	seed;			/* seed used for file decrypt. */
	unsigned int	blocks;			/* number of blocks in the file (inclusive the last noncomplete one). */
	unsigned int	uncompressed_offset;	/* position in file after extraction (bytes copied). */
	unsigned int	*compressed_offset;	/* position of each file block (only for compressed files). */
	unsigned int	file;			/* file number which is actually processed. */
	unsigned char	*block_buffer;		/* buffer (cache) for file block. */
	unsigned char	*in_buf;		/* input buffer (compressed data for compressed files). */
	unsigned char	*out_buf;		/* output buffer (uncompressed data for extracted files). */
	mpq_hash_s	*mpq_hash;		/* hash table entry. */
	mpq_block_s	*mpq_block;		/* file block pointer. */
} mpq_file_s;

/* filelist structure. */
typedef struct {
	char		**file_names;		/* file name for archive members. */
	unsigned int	*block_table_indices;	/* pointer which stores the mapping for file number to block entry. */
	unsigned int	*hash_table_indices;	/* pointer which stores the mapping for file number to hash entry. */
} mpq_list_s;

/* archive structure used since diablo 1.00 by blizzard. */
typedef struct {

	/* generic file information. */
	char		filename[PATH_MAX];	/* archive file name. */
	int		fd;			/* file handle. */

	/* generic position information. */
	unsigned int	block_size;		/* size of the mpq block. */
	unsigned int	block_offset;		/* position of loaded block in the file. */
	unsigned int	archive_offset;		/* archive position in the file. */

	/* archive related buffers and tables. */
	unsigned int	mpq_buffer[0x500];	/* mpq encryption and decryption buffer. */
	mpq_header_s	*mpq_header;		/* mpq file header. */
	mpq_hash_s	*mpq_hash;		/* hash table. */
	mpq_block_s	*mpq_block;		/* block table. */
	mpq_file_s	*mpq_file;		/* pointer to file which is processed. */

	/* non archive structure related members. */
	mpq_list_s	*mpq_list;		/* handle to filelist (in most cases this is the last file in the archive). */
	unsigned int	files;			/* number of files in archive, which could be extracted. */
} mpq_archive_s;

/* generic information about library. */
extern unsigned char *libmpq__version();

/* generic mpq archive information. */
extern int libmpq__archive_open(mpq_archive_s *mpq_archive, const char *mpq_filename);
extern int libmpq__archive_close(mpq_archive_s *mpq_archive);
extern int libmpq__archive_info(mpq_archive_s *mpq_archive, unsigned int infotype);

/* generic file information. */
extern int libmpq__file_open(mpq_archive_s *mpq_archive, const unsigned int number);
extern int libmpq__file_close(mpq_archive_s *mpq_archive);
extern int libmpq__file_info(mpq_archive_s *mpq_archive, unsigned int infotype, const unsigned int number);
extern char *libmpq__file_name(mpq_archive_s *mpq_archive, const unsigned int number);
extern int libmpq__file_number(mpq_archive_s *mpq_archive, const char *name);
extern int libmpq__file_extract(mpq_archive_s *mpq_archive);

#endif						/* _MPQ_H */
