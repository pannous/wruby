/*
 * This file is loading the irep
 * Ruby GEM code.
 *
 * IMPORTANT:
 *   This file was generated!
 *   All manual changes will get lost.
 */
#include <stdio.h>
#include <stdlib.h>
#include <mruby.h>
#include <mruby/irep.h>
#include <mruby/variable.h>
/*
 * This file contains a test code for mruby-object-ext gem.
 *
 * IMPORTANT:
 *   This file was generated!
 *   All manual changes will get lost.
 */
extern const uint8_t mrbtest_assert_irep[];
/* dumped in little endian order.
   use `mrbc -E` option for big endian CPU. */
#include <stdint.h>
extern const uint8_t gem_test_irep_mruby_object_ext_0[];
const uint8_t
#if defined __GNUC__
__attribute__((aligned(4)))
#elif defined _MSC_VER
__declspec(align(4))
#endif
gem_test_irep_mruby_object_ext_0[] = {
0x45,0x54,0x49,0x52,0x30,0x30,0x30,0x35,0x4d,0x08,0x00,0x00,0x02,0xe5,0x4d,0x41,
0x54,0x5a,0x30,0x30,0x30,0x30,0x49,0x52,0x45,0x50,0x00,0x00,0x01,0x7c,0x30,0x30,
0x30,0x32,0x00,0x00,0x01,0x3c,0x00,0x01,0x00,0x04,0x00,0x03,0x00,0x00,0x00,0x34,
0x10,0x01,0x4f,0x02,0x00,0x55,0x03,0x00,0x2f,0x01,0x00,0x01,0x10,0x01,0x4f,0x02,
0x01,0x2e,0x01,0x01,0x01,0x23,0x01,0x00,0x25,0x10,0x01,0x4f,0x02,0x02,0x55,0x03,
0x01,0x2f,0x01,0x00,0x01,0x10,0x01,0x4f,0x02,0x03,0x55,0x03,0x02,0x2f,0x01,0x00,
0x01,0x37,0x01,0x67,0x00,0x00,0x00,0x04,0x00,0x00,0x0d,0x4e,0x69,0x6c,0x43,0x6c,
0x61,0x73,0x73,0x23,0x74,0x6f,0x5f,0x61,0x00,0x00,0x05,0x46,0x6c,0x6f,0x61,0x74,
0x00,0x00,0x0d,0x4e,0x69,0x6c,0x43,0x6c,0x61,0x73,0x73,0x23,0x74,0x6f,0x5f,0x66,
0x00,0x00,0x0d,0x4e,0x69,0x6c,0x43,0x6c,0x61,0x73,0x73,0x23,0x74,0x6f,0x5f,0x69,
0x00,0x00,0x00,0x02,0x00,0x06,0x61,0x73,0x73,0x65,0x72,0x74,0x00,0x00,0x0e,0x63,
0x6c,0x61,0x73,0x73,0x5f,0x64,0x65,0x66,0x69,0x6e,0x65,0x64,0x3f,0x00,0x00,0x00,
0x00,0x74,0x00,0x01,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x11,0x10,0x01,0x46,0x02,
0x00,0x0f,0x03,0x2e,0x03,0x00,0x00,0x2e,0x01,0x01,0x02,0x37,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x02,0x00,0x04,0x74,0x6f,0x5f,0x61,0x00,0x00,0x0c,0x61,0x73,
0x73,0x65,0x72,0x74,0x5f,0x65,0x71,0x75,0x61,0x6c,0x00,0x00,0x00,0x00,0x8d,0x00,
0x01,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x10,0x01,0x02,0x02,
0x00,0x0f,0x03,0x2e,0x03,0x00,0x00,0x2e,0x01,0x01,0x02,0x37,0x01,0x00,0x00,0x00,
0x01,0x02,0x00,0x16,0x30,0x2e,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
0x30,0x30,0x30,0x30,0x30,0x30,0x65,0x2b,0x30,0x30,0x00,0x00,0x00,0x02,0x00,0x04,
0x74,0x6f,0x5f,0x66,0x00,0x00,0x0c,0x61,0x73,0x73,0x65,0x72,0x74,0x5f,0x65,0x71,
0x75,0x61,0x6c,0x00,0x00,0x00,0x00,0x70,0x00,0x01,0x00,0x05,0x00,0x00,0x00,0x00,
0x00,0x10,0x00,0x00,0x10,0x01,0x06,0x02,0x0f,0x03,0x2e,0x03,0x00,0x00,0x2e,0x01,
0x01,0x02,0x37,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x04,0x74,0x6f,
0x5f,0x69,0x00,0x00,0x0c,0x61,0x73,0x73,0x65,0x72,0x74,0x5f,0x65,0x71,0x75,0x61,
0x6c,0x00,0x44,0x42,0x47,0x00,0x00,0x00,0x01,0x4b,0x00,0x01,0x00,0x2f,0x2f,0x6f,
0x70,0x74,0x2f,0x6d,0x72,0x75,0x62,0x79,0x2f,0x6d,0x72,0x62,0x67,0x65,0x6d,0x73,
0x2f,0x6d,0x72,0x75,0x62,0x79,0x2d,0x6f,0x62,0x6a,0x65,0x63,0x74,0x2d,0x65,0x78,
0x74,0x2f,0x74,0x65,0x73,0x74,0x2f,0x6e,0x69,0x6c,0x2e,0x72,0x62,0x00,0x00,0x00,
0x79,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x34,0x00,0x00,0x01,
0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,
0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x05,0x00,0x05,
0x00,0x05,0x00,0x05,0x00,0x05,0x00,0x05,0x00,0x05,0x00,0x05,0x00,0x05,0x00,0x05,
0x00,0x05,0x00,0x05,0x00,0x05,0x00,0x05,0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x09,
0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x09,
0x00,0x09,0x00,0x09,0x00,0x09,0x00,0x00,0x00,0x33,0x00,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x00,0x00,0x33,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x06,0x00,0x06,0x00,0x06,
0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,
0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x31,
0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x0a,0x00,
0x0a,0x00,0x0a,0x00,0x0a,0x00,0x0a,0x00,0x0a,0x00,0x0a,0x00,0x0a,0x00,0x0a,0x00,
0x0a,0x00,0x0a,0x00,0x0a,0x00,0x0a,0x00,0x0a,0x00,0x0a,0x00,0x0a,0x45,0x4e,0x44,
0x00,0x00,0x00,0x00,0x08,
};
/* dumped in little endian order.
   use `mrbc -E` option for big endian CPU. */
#include <stdint.h>
extern const uint8_t gem_test_irep_mruby_object_ext_1[];
const uint8_t
#if defined __GNUC__
__attribute__((aligned(4)))
#elif defined _MSC_VER
__declspec(align(4))
#endif
gem_test_irep_mruby_object_ext_1[] = {
0x45,0x54,0x49,0x52,0x30,0x30,0x30,0x35,0x94,0xc6,0x00,0x00,0x0d,0x43,0x4d,0x41,
0x54,0x5a,0x30,0x30,0x30,0x30,0x49,0x52,0x45,0x50,0x00,0x00,0x06,0xc9,0x30,0x30,
0x30,0x32,0x00,0x00,0x01,0x22,0x00,0x01,0x00,0x04,0x00,0x03,0x00,0x00,0x00,0x27,
0x10,0x01,0x4f,0x02,0x00,0x55,0x03,0x00,0x2f,0x01,0x00,0x01,0x10,0x01,0x4f,0x02,
0x01,0x55,0x03,0x01,0x2f,0x01,0x00,0x01,0x10,0x01,0x4f,0x02,0x02,0x55,0x03,0x02,
0x2f,0x01,0x00,0x01,0x37,0x01,0x67,0x00,0x00,0x00,0x03,0x00,0x00,0x14,0x4f,0x62,
0x6a,0x65,0x63,0x74,0x23,0x69,0x6e,0x73,0x74,0x61,0x6e,0x63,0x65,0x5f,0x65,0x78,
0x65,0x63,0x00,0x00,0x0a,0x4f,0x62,0x6a,0x65,0x63,0x74,0x23,0x74,0x61,0x70,0x00,
0x00,0x3c,0x69,0x6e,0x73,0x74,0x61,0x6e,0x63,0x65,0x5f,0x65,0x78,0x65,0x63,0x20,
0x6f,0x6e,0x20,0x70,0x72,0x69,0x6d,0x69,0x74,0x69,0x76,0x65,0x73,0x20,0x77,0x69,
0x74,0x68,0x20,0x63,0x6c,0x61,0x73,0x73,0x20,0x61,0x6e,0x64,0x20,0x6d,0x6f,0x64,
0x75,0x6c,0x65,0x20,0x64,0x65,0x66,0x69,0x6e,0x69,0x74,0x69,0x6f,0x6e,0x00,0x00,
0x00,0x01,0x00,0x06,0x61,0x73,0x73,0x65,0x72,0x74,0x00,0x00,0x00,0x00,0xfd,0x00,
0x02,0x00,0x07,0x00,0x02,0x00,0x00,0x00,0x2b,0x00,0x00,0x00,0x0f,0x02,0x0f,0x03,
0x5a,0x02,0x00,0x5c,0x02,0x00,0x1b,0x02,0x00,0x2e,0x02,0x01,0x00,0x01,0x01,0x02,
0x10,0x02,0x03,0x03,0x68,0x01,0x04,0x01,0x0b,0x05,0x55,0x06,0x01,0x2f,0x04,0x02,
0x01,0x2e,0x02,0x03,0x02,0x37,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,
0x0f,0x4b,0x6c,0x61,0x73,0x73,0x57,0x69,0x74,0x68,0x53,0x65,0x63,0x72,0x65,0x74,
0x00,0x00,0x03,0x6e,0x65,0x77,0x00,0x00,0x0d,0x69,0x6e,0x73,0x74,0x61,0x6e,0x63,
0x65,0x5f,0x65,0x78,0x65,0x63,0x00,0x00,0x0c,0x61,0x73,0x73,0x65,0x72,0x74,0x5f,
0x65,0x71,0x75,0x61,0x6c,0x00,0x00,0x00,0x00,0x5b,0x00,0x01,0x00,0x03,0x00,0x01,
0x00,0x00,0x00,0x0d,0x61,0x01,0x56,0x02,0x00,0x5d,0x01,0x00,0x0e,0x01,0x00,0x37,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x0a,0x69,0x6e,0x69,0x74,0x69,
0x61,0x6c,0x69,0x7a,0x65,0x00,0x00,0x00,0x00,0x54,0x00,0x02,0x00,0x03,0x00,0x00,
0x00,0x00,0x00,0x0c,0x33,0x00,0x00,0x00,0x03,0x02,0x63,0x18,0x02,0x00,0x37,0x02,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x07,0x40,0x73,0x65,0x63,0x72,0x65,
0x74,0x00,0x00,0x00,0x00,0x5c,0x00,0x03,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x0e,
0x33,0x04,0x00,0x00,0x17,0x03,0x00,0x01,0x04,0x01,0x3b,0x03,0x37,0x03,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x07,0x40,0x73,0x65,0x63,0x72,0x65,0x74,0x00,
0x00,0x00,0x02,0x95,0x00,0x02,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x70,0x00,0x00,
0x46,0x02,0x00,0x01,0x01,0x02,0x07,0x02,0x03,0x03,0x0a,0x57,0x02,0x55,0x03,0x00,
0x2f,0x02,0x00,0x00,0x2e,0x02,0x01,0x00,0x55,0x03,0x01,0x2f,0x02,0x00,0x00,0x55,
0x03,0x02,0x2f,0x02,0x02,0x00,0x55,0x03,0x03,0x2f,0x02,0x00,0x00,0x55,0x03,0x04,
0x2f,0x02,0x03,0x00,0x55,0x03,0x05,0x2f,0x02,0x00,0x00,0x10,0x02,0x4f,0x03,0x00,
0x4f,0x04,0x01,0x4f,0x05,0x02,0x4f,0x06,0x03,0x46,0x03,0x04,0x01,0x04,0x01,0x2e,
0x02,0x04,0x02,0x10,0x02,0x0e,0x03,0x05,0x1b,0x04,0x06,0x55,0x05,0x06,0x2f,0x04,
0x07,0x00,0x2e,0x04,0x07,0x00,0x2e,0x04,0x08,0x00,0x2e,0x02,0x04,0x02,0x37,0x02,
0x00,0x00,0x00,0x04,0x00,0x00,0x0f,0x6f,0x72,0x69,0x67,0x69,0x6e,0x61,0x6c,0x3a,
0x20,0x31,0x2e,0x2e,0x31,0x30,0x00,0x00,0x26,0x61,0x72,0x72,0x61,0x79,0x3a,0x20,
0x5b,0x31,0x2c,0x20,0x32,0x2c,0x20,0x33,0x2c,0x20,0x34,0x2c,0x20,0x35,0x2c,0x20,
0x36,0x2c,0x20,0x37,0x2c,0x20,0x38,0x2c,0x20,0x39,0x2c,0x20,0x31,0x30,0x5d,0x00,
0x00,0x17,0x65,0x76,0x65,0x6e,0x73,0x3a,0x20,0x5b,0x32,0x2c,0x20,0x34,0x2c,0x20,
0x36,0x2c,0x20,0x38,0x2c,0x20,0x31,0x30,0x5d,0x00,0x00,0x1d,0x73,0x71,0x75,0x61,
0x72,0x65,0x73,0x3a,0x20,0x5b,0x34,0x2c,0x20,0x31,0x36,0x2c,0x20,0x33,0x36,0x2c,
0x20,0x36,0x34,0x2c,0x20,0x31,0x30,0x30,0x5d,0x00,0x00,0x00,0x09,0x00,0x03,0x74,
0x61,0x70,0x00,0x00,0x04,0x74,0x6f,0x5f,0x61,0x00,0x00,0x06,0x73,0x65,0x6c,0x65,
0x63,0x74,0x00,0x00,0x03,0x6d,0x61,0x70,0x00,0x00,0x0c,0x61,0x73,0x73,0x65,0x72,
0x74,0x5f,0x65,0x71,0x75,0x61,0x6c,0x00,0x00,0x06,0x74,0x61,0x70,0x5f,0x6f,0x6b,
0x00,0x00,0x05,0x43,0x6c,0x61,0x73,0x73,0x00,0x00,0x03,0x6e,0x65,0x77,0x00,0x00,
0x01,0x6d,0x00,0x00,0x00,0x00,0xb5,0x00,0x03,0x00,0x07,0x00,0x00,0x00,0x00,0x00,
0x1f,0x00,0x00,0x00,0x33,0x04,0x00,0x00,0x1f,0x03,0x01,0x00,0x4f,0x04,0x00,0x01,
0x05,0x01,0x2e,0x05,0x00,0x00,0x50,0x04,0x4f,0x05,0x01,0x50,0x04,0x2e,0x03,0x01,
0x01,0x37,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x0a,0x6f,0x72,0x69,0x67,0x69,0x6e,
0x61,0x6c,0x3a,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x07,0x69,0x6e,0x73,
0x70,0x65,0x63,0x74,0x00,0x00,0x02,0x3c,0x3c,0x00,0x00,0x00,0x00,0xb2,0x00,0x03,
0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x1f,0x33,0x04,0x00,0x00,0x1f,0x03,0x01,0x00,
0x4f,0x04,0x00,0x01,0x05,0x01,0x2e,0x05,0x00,0x00,0x50,0x04,0x4f,0x05,0x01,0x50,
0x04,0x2e,0x03,0x01,0x01,0x37,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x07,0x61,0x72,
0x72,0x61,0x79,0x3a,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x07,0x69,0x6e,
0x73,0x70,0x65,0x63,0x74,0x00,0x00,0x02,0x3c,0x3c,0x00,0x00,0x00,0x00,0x6a,0x00,
0x03,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x33,0x04,0x00,0x00,
0x01,0x03,0x01,0x08,0x04,0x2e,0x03,0x00,0x01,0x06,0x04,0x41,0x03,0x37,0x03,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x25,0x00,0x00,0x00,0x00,0xb2,0x00,
0x03,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x33,0x04,0x00,0x00,
0x1f,0x03,0x01,0x00,0x4f,0x04,0x00,0x01,0x05,0x01,0x2e,0x05,0x00,0x00,0x50,0x04,
0x4f,0x05,0x01,0x50,0x04,0x2e,0x03,0x01,0x01,0x37,0x03,0x00,0x00,0x00,0x02,0x00,
0x00,0x07,0x65,0x76,0x65,0x6e,0x73,0x3a,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
0x00,0x07,0x69,0x6e,0x73,0x70,0x65,0x63,0x74,0x00,0x00,0x02,0x3c,0x3c,0x00,0x00,
0x00,0x00,0x52,0x00,0x03,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,
0x33,0x04,0x00,0x00,0x01,0x03,0x01,0x01,0x04,0x01,0x3f,0x03,0x37,0x03,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xb4,0x00,0x03,0x00,0x07,0x00,0x00,
0x00,0x00,0x00,0x1f,0x33,0x04,0x00,0x00,0x1f,0x03,0x01,0x00,0x4f,0x04,0x00,0x01,
0x05,0x01,0x2e,0x05,0x00,0x00,0x50,0x04,0x4f,0x05,0x01,0x50,0x04,0x2e,0x03,0x01,
0x01,0x37,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x09,0x73,0x71,0x75,0x61,0x72,0x65,
0x73,0x3a,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x07,0x69,0x6e,0x73,0x70,
0x65,0x63,0x74,0x00,0x00,0x02,0x3c,0x3c,0x00,0x00,0x00,0x00,0x52,0x00,0x01,0x00,
0x03,0x00,0x01,0x00,0x00,0x00,0x0d,0x00,0x61,0x01,0x56,0x02,0x00,0x5d,0x01,0x00,
0x0e,0x01,0x00,0x37,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x6d,
0x00,0x00,0x00,0x00,0x5c,0x00,0x02,0x00,0x04,0x00,0x01,0x00,0x00,0x00,0x0f,0x00,
0x33,0x00,0x00,0x00,0x10,0x02,0x55,0x03,0x00,0x2f,0x02,0x00,0x00,0x37,0x02,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x03,0x74,0x61,0x70,0x00,0x00,0x00,0x00,
0x3f,0x00,0x01,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x0e,0x01,0x00,0x38,
0x01,0x37,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x06,0x74,0x61,0x70,
0x5f,0x6f,0x6b,0x00,0x00,0x00,0x01,0x34,0x00,0x01,0x00,0x05,0x00,0x04,0x00,0x00,
0x00,0x3c,0x00,0x00,0x2a,0x00,0x0f,0x01,0x0f,0x02,0x5a,0x01,0x00,0x5c,0x01,0x01,
0x10,0x01,0x1b,0x02,0x01,0x1b,0x03,0x00,0x1d,0x03,0x02,0x2e,0x01,0x03,0x02,0x2b,
0x01,0x2a,0x02,0x0f,0x01,0x0f,0x02,0x5a,0x01,0x00,0x5c,0x01,0x03,0x10,0x01,0x1b,
0x02,0x04,0x1b,0x03,0x00,0x1d,0x03,0x02,0x2e,0x01,0x03,0x02,0x2b,0x01,0x37,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x01,0x41,0x00,0x00,0x05,0x43,0x6c,
0x61,0x73,0x73,0x00,0x00,0x01,0x42,0x00,0x00,0x0e,0x61,0x73,0x73,0x65,0x72,0x74,
0x5f,0x6b,0x69,0x6e,0x64,0x5f,0x6f,0x66,0x00,0x00,0x06,0x4d,0x6f,0x64,0x75,0x6c,
0x65,0x00,0x00,0x00,0x00,0x66,0x00,0x01,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x0c,
0x1b,0x01,0x00,0x0e,0x02,0x01,0x2e,0x01,0x02,0x01,0x37,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0x00,0x06,0x4f,0x62,0x6a,0x65,0x63,0x74,0x00,0x00,0x01,0x41,
0x00,0x00,0x0c,0x72,0x65,0x6d,0x6f,0x76,0x65,0x5f,0x63,0x6f,0x6e,0x73,0x74,0x00,
0x00,0x00,0x00,0x56,0x00,0x01,0x00,0x03,0x00,0x01,0x00,0x00,0x00,0x0b,0x00,0x00,
0x07,0x01,0x55,0x02,0x00,0x2f,0x01,0x00,0x00,0x37,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x00,0x0d,0x69,0x6e,0x73,0x74,0x61,0x6e,0x63,0x65,0x5f,0x65,0x78,
0x65,0x63,0x00,0x00,0x00,0x00,0x42,0x00,0x01,0x00,0x03,0x00,0x00,0x00,0x00,0x00,
0x09,0x00,0x00,0x00,0x0f,0x01,0x0f,0x02,0x5a,0x01,0x00,0x37,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x42,0x00,0x00,0x00,0x00,0x66,0x00,0x01,0x00,
0x04,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x1b,0x01,0x00,0x0e,0x02,0x01,0x2e,0x01,
0x02,0x01,0x37,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x06,0x4f,0x62,
0x6a,0x65,0x63,0x74,0x00,0x00,0x01,0x41,0x00,0x00,0x0c,0x72,0x65,0x6d,0x6f,0x76,
0x65,0x5f,0x63,0x6f,0x6e,0x73,0x74,0x00,0x00,0x00,0x00,0x56,0x00,0x01,0x00,0x03,
0x00,0x01,0x00,0x00,0x00,0x0b,0x00,0x00,0x07,0x01,0x55,0x02,0x00,0x2f,0x01,0x00,
0x00,0x37,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x0d,0x69,0x6e,0x73,
0x74,0x61,0x6e,0x63,0x65,0x5f,0x65,0x78,0x65,0x63,0x00,0x00,0x00,0x00,0x3a,0x00,
0x01,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x0f,0x01,0x5b,0x01,
0x00,0x37,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x42,0x00,0x44,
0x42,0x47,0x00,0x00,0x00,0x05,0xfa,0x00,0x01,0x00,0x32,0x2f,0x6f,0x70,0x74,0x2f,
0x6d,0x72,0x75,0x62,0x79,0x2f,0x6d,0x72,0x62,0x67,0x65,0x6d,0x73,0x2f,0x6d,0x72,
0x75,0x62,0x79,0x2d,0x6f,0x62,0x6a,0x65,0x63,0x74,0x2d,0x65,0x78,0x74,0x2f,0x74,
0x65,0x73,0x74,0x2f,0x6f,0x62,0x6a,0x65,0x63,0x74,0x2e,0x72,0x62,0x00,0x00,0x00,
0x5f,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x27,0x00,0x00,0x01,
0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x0b,0x00,0x0b,0x00,0x0b,0x00,0x0b,0x00,0x0b,
0x00,0x0b,0x00,0x0b,0x00,0x0b,0x00,0x0b,0x00,0x0b,0x00,0x0b,0x00,0x0b,0x00,0x18,
0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,
0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x00,0x00,0x67,
0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2b,0x00,0x00,0x02,0x00,
0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,
0x07,0x00,0x07,0x00,0x07,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,
0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,
0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,
0x08,0x00,0x08,0x00,0x00,0x00,0x2b,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0d,0x00,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,
0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,
0x00,0x29,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x00,
0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,
0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x00,0x00,0x2d,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x00,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,
0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,
0x00,0x08,0x00,0x08,0x00,0x00,0x00,0xf1,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x70,0x00,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,0x0c,0x00,
0x0c,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x10,0x00,0x10,0x00,
0x10,0x00,0x10,0x00,0x10,0x00,0x11,0x00,0x11,0x00,0x11,0x00,0x12,0x00,0x12,0x00,
0x12,0x00,0x13,0x00,0x13,0x00,0x13,0x00,0x13,0x00,0x13,0x00,0x13,0x00,0x14,0x00,
0x14,0x00,0x14,0x00,0x14,0x00,0x14,0x00,0x14,0x00,0x14,0x00,0x15,0x00,0x15,0x00,
0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,
0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,
0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,
0x15,0x00,0x15,0x00,0x15,0x00,0x00,0x00,0x4f,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x1f,0x00,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,
0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,
0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,
0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,
0x00,0x0d,0x00,0x0d,0x00,0x00,0x00,0x4f,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x1f,0x00,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x00,0x00,0x37,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x13,0x00,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,
0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,
0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x00,0x00,0x4f,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0x00,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x00,0x00,0x2d,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x00,0x00,0x0d,0x00,0x0d,0x00,0x0d,
0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,
0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x00,0x00,0x4f,0x00,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x1f,0x00,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,0x0d,0x00,
0x0d,0x00,0x0d,0x00,0x0d,0x00,0x00,0x00,0x2b,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0d,0x00,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,
0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,
0x00,0x00,0x00,0x2f,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,
0x00,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,
0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,
0x00,0x00,0x1f,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,
0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x15,0x00,0x00,
0x00,0x89,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x00,0x00,
0x24,0x00,0x24,0x00,0x1a,0x00,0x1a,0x00,0x1a,0x00,0x1a,0x00,0x1a,0x00,0x1a,0x00,
0x1a,0x00,0x1a,0x00,0x1a,0x00,0x1a,0x00,0x21,0x00,0x21,0x00,0x21,0x00,0x21,0x00,
0x21,0x00,0x21,0x00,0x21,0x00,0x21,0x00,0x21,0x00,0x21,0x00,0x21,0x00,0x21,0x00,
0x21,0x00,0x21,0x00,0x21,0x00,0x21,0x00,0x21,0x00,0x31,0x00,0x31,0x00,0x27,0x00,
0x27,0x00,0x27,0x00,0x27,0x00,0x27,0x00,0x27,0x00,0x27,0x00,0x27,0x00,0x27,0x00,
0x27,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,
0x2e,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,
0x2e,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,0x00,0x00,0x29,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x00,0x23,0x00,0x23,0x00,0x23,0x00,0x23,
0x00,0x23,0x00,0x23,0x00,0x23,0x00,0x23,0x00,0x23,0x00,0x23,0x00,0x23,0x00,0x23,
0x00,0x00,0x00,0x27,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0b,
0x00,0x00,0x1b,0x00,0x1b,0x00,0x1b,0x00,0x1b,0x00,0x1b,0x00,0x1b,0x00,0x1b,0x00,
0x1b,0x00,0x1b,0x00,0x1b,0x00,0x1b,0x00,0x00,0x00,0x23,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x1c,0x00,0x1c,0x00,0x1c,0x00,0x1c,
0x00,0x1c,0x00,0x1c,0x00,0x1c,0x00,0x1c,0x00,0x1c,0x00,0x00,0x00,0x29,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00,0x00,0x30,0x00,0x30,0x00,
0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,
0x30,0x00,0x30,0x00,0x00,0x00,0x27,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0b,0x00,0x00,0x28,0x00,0x28,0x00,0x28,0x00,0x28,0x00,0x28,0x00,0x28,
0x00,0x28,0x00,0x28,0x00,0x28,0x00,0x28,0x00,0x28,0x00,0x00,0x00,0x1f,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x29,0x00,0x29,0x00,
0x29,0x00,0x29,0x00,0x29,0x00,0x29,0x00,0x29,0x4c,0x56,0x41,0x52,0x00,0x00,0x00,
0x62,0x00,0x00,0x00,0x04,0x00,0x01,0x6b,0x00,0x01,0x26,0x00,0x01,0x78,0x00,0x03,
0x72,0x65,0x74,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x02,0x00,0x01,0x00,
0x01,0x00,0x02,0x00,0x03,0x00,0x01,0x00,0x02,0x00,0x01,0x00,0x01,0x00,0x02,0x00,
0x02,0x00,0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00,0x01,0x00,0x01,0x00,0x02,0x00,
0x02,0x00,0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00,0x01,0x00,0x01,0x00,0x02,0x00,
0x02,0x00,0x01,0x00,0x01,0x00,0x02,0x00,0x01,0x00,0x01,0x45,0x4e,0x44,0x00,0x00,
0x00,0x00,0x08,
};
void _mruby_object_ext_gem_test(state *mrb);
void GENERATED_TMP__mruby_object_ext_gem_init(state *mrb);
void GENERATED_TMP__mruby_object_ext_gem_final(state *mrb);
void _init_test_driver(state *mrb, _bool verbose);
void _t_pass_result(state *dst, state *src);
void GENERATED_TMP__mruby_object_ext_gem_test(state *mrb) {
  state *mrb2;
  int ai;
  ai = _gc_arena_save(mrb);
  mrb2 = _open_core(_default_allocf, NULL);
  if (mrb2 == NULL) {
    fprintf(stderr, "Invalid state, exiting %s", __FUNCTION__);
    exit(EXIT_FAILURE);
  }
  GENERATED_TMP__mruby_object_ext_gem_init(mrb2);
  _state_atexit(mrb2, GENERATED_TMP__mruby_object_ext_gem_final);
  _init_test_driver(mrb2, _test(_gv_get(mrb, _intern_lit(mrb, "$mrbtest_verbose"))));
  _load_irep(mrb2, mrbtest_assert_irep);
  if (mrb2->exc) {
    _print_error(mrb2);
    _close(mrb2);
    exit(EXIT_FAILURE);
  }
  _const_set(mrb2, _obj_value(mrb2->object_class), _intern_lit(mrb2, "GEMNAME"), _str_new(mrb2, "mruby-object-ext", 16));
  _load_irep(mrb2, gem_test_irep_mruby_object_ext_0);
  
  _t_pass_result(mrb, mrb2);
  _close(mrb2);
  _gc_arena_restore(mrb, ai);
  ai = _gc_arena_save(mrb);
  mrb2 = _open_core(_default_allocf, NULL);
  if (mrb2 == NULL) {
    fprintf(stderr, "Invalid state, exiting %s", __FUNCTION__);
    exit(EXIT_FAILURE);
  }
  GENERATED_TMP__mruby_object_ext_gem_init(mrb2);
  _state_atexit(mrb2, GENERATED_TMP__mruby_object_ext_gem_final);
  _init_test_driver(mrb2, _test(_gv_get(mrb, _intern_lit(mrb, "$mrbtest_verbose"))));
  _load_irep(mrb2, mrbtest_assert_irep);
  if (mrb2->exc) {
    _print_error(mrb2);
    _close(mrb2);
    exit(EXIT_FAILURE);
  }
  _const_set(mrb2, _obj_value(mrb2->object_class), _intern_lit(mrb2, "GEMNAME"), _str_new(mrb2, "mruby-object-ext", 16));
  _load_irep(mrb2, gem_test_irep_mruby_object_ext_1);
  
  _t_pass_result(mrb, mrb2);
  _close(mrb2);
  _gc_arena_restore(mrb, ai);
}