#include "../header.h"
#include "classes.h"
std::vector<game::s_skin> skins = {
	{ 1, { true, true, true, true, true, false } },
	{ 2, { true, true, true, true, true, false } },
	{ 3, { true, true, true, true, true, false } },
	{ 4, { true, true, true, true, true, false } },
	{ 5, { true, true, true, true, true, false } },
	{ 6, { true, true, true, true, true, false } },
	{ 8, { false, false, false, false, true, false } },
	{ 9, { true, true, true, true, true, false } },
	{ 10, { true, true, true, true, true, false } },
	{ 11, { true, true, true, true, true, false } },
	{ 12, { true, true, true, true, true, false } },
	{ 13, { true, true, true, true, true, false } },
	{ 14, { true, true, true, true, true, false } },
	{ 15, { true, true, true, true, true, false } },
	{ 18, { true, true, true, true, true, false } },
	{ 20, { true, true, true, true, true, false } },
	{ 21, { true, true, true, true, true, false } },
	{ 22, { true, true, true, true, true, false } },
	{ 23, { true, true, true, true, true, false } },
	{ 24, { true, true, true, true, true, false } },
	{ 25, { true, true, true, true, true, false } },
	{ 26, { true, true, true, true, true, false } },
	{ 27, { true, true, true, true, true, false } },
	{ 28, { true, true, true, true, true, false } },
	{ 29, { true, true, true, true, true, false } },
	{ 31, { true, true, true, true, true, false } },
	{ 33, { true, true, true, true, true, false } },
	{ 34, { true, true, true, true, true, false } },
	{ 35, { true, true, true, true, true, false } },
	{ 36, { true, true, true, true, true, false } },
	{ 37, { true, false, false, false, false, false } },
	{ 39, { true, true, true, true, true, false } },
	{ 40, { true, true, true, true, true, false } },
	{ 41, { true, true, true, true, true, false } },
	{ 42, { true, true, true, true, true, false } },
	{ 43, { true, true, true, true, true, false } },
	{ 44, { true, true, true, true, true, false } },
	{ 45, { true, true, true, true, true, false } },
	{ 46, { true, true, true, true, true, false } },
	{ 49, { false, false, false, false, true, false } },
	{ 50, { false, false, false, false, true, false } },
	{ 51, { true, true, true, true, true, false } },
	{ 52, { true, true, true, true, true, false } },
	{ 55, { false, true, false, false, false, false } },
	{ 56, { false, true, false, false, false, false } },
	{ 57, { true, true, true, true, true, false } },
	{ 58, { true, true, true, true, true, false } },
	{ 61, { true, true, true, true, true, false } },
	{ 62, { true, true, true, true, true, false } },
	{ 63, { true, true, true, true, true, false } },
	{ 64, { true, true, true, true, true, false } },
	{ 65, { true, true, true, true, true, false } },
	{ 66, { true, true, true, true, true, false } },
	{ 67, { true, true, true, true, true, false } },
	{ 68, { true, true, true, true, true, false } },
	{ 69, { true, true, true, true, true, false } },
	{ 70, { true, true, true, true, true, false } },
	{ 71, { true, true, true, true, true, false } },
	{ 72, { true, true, true, true, true, false } },
	{ 73, { true, true, true, true, true, false } },
	{ 74, { true, true, true, true, true, false } },
	{ 75, { true, true, true, true, true, false } },
	{ 76, { true, true, true, true, true, false } },
	{ 77, { true, true, true, true, true, false } },
	{ 78, { true, true, true, true, true, false } },
	{ 79, { true, true, true, true, true, false } },
	{ 80, { true, true, true, true, true, false } },
	{ 81, { true, true, true, true, true, false } },
	{ 82, { true, true, true, true, true, false } },
	{ 83, { true, true, true, true, true, false } },
	{ 84, { true, true, true, true, true, false } },
	{ 85, { true, true, true, true, true, false } },
	{ 86, { true, true, true, true, true, false } },
	{ 87, { true, true, true, true, true, false } },
	{ 88, { true, true, true, true, true, false } },
	{ 89, { true, true, true, true, true, false } },
	{ 90, { true, true, true, true, true, false } },
	{ 91, { true, true, true, true, true, false } },
	{ 92, { true, true, true, true, true, false } },
	{ 93, { true, true, true, true, true, false } },
	{ 94, { true, true, true, true, true, false } },
	{ 95, { true, true, true, true, true, false } },
	{ 96, { true, true, true, true, true, false } },
	{ 97, { true, true, true, true, true, false } },
	{ 98, { true, true, true, true, true, false } },
	{ 99, { true, true, true, true, true, false } },
	{ 100, { true, true, true, true, true, false } },
	{ 101, { true, true, true, true, true, false } },
	{ 102, { true, true, true, true, true, false } },
	{ 103, { true, true, true, true, true, false } },
	{ 104, { true, true, true, true, true, false } },
	{ 105, { true, true, true, true, true, false } },
	{ 106, { true, true, true, true, true, false } },
	{ 107, { true, true, true, true, true, false } },
	{ 108, { true, true, true, true, true, false } },
	{ 109, { true, true, true, true, true, false } },
	{ 110, { true, true, true, true, true, false } },
	{ 111, { true, true, true, true, true, false } },
	{ 112, { true, true, true, true, true, false } },
	{ 113, { true, true, true, true, true, false } },
	{ 114, { true, true, true, true, true, false } },
	{ 115, { true, true, true, true, true, false } },
	{ 116, { true, true, true, true, true, false } },
	{ 117, { true, true, true, true, true, false } },
	{ 118, { true, true, true, true, true, false } },
	{ 119, { true, true, true, true, true, false } },
	{ 120, { true, true, true, true, true, false } },
	{ 121, { true, true, true, true, true, false } },
	{ 122, { true, true, true, true, true, false } },
	{ 123, { true, true, true, true, true, false } },
	{ 124, { true, true, true, true, true, false } },
	{ 125, { true, true, true, true, true, false } },
	{ 126, { true, true, true, true, true, false } },
	{ 127, { true, true, true, true, true, false } },
	{ 128, { true, true, true, true, true, false } },
	{ 129, { true, true, true, true, true, false } },
	{ 130, { true, true, true, true, true, false } },
	{ 131, { true, true, true, true, true, false } },
	{ 132, { true, true, true, true, true, false } },
	{ 133, { true, true, true, true, true, false } },
	{ 134, { true, true, true, true, true, false } },
	{ 135, { true, true, true, true, true, false } },
	{ 136, { true, true, true, true, true, false } },
	{ 137, { true, true, true, true, true, false } },
	{ 138, { true, true, true, true, true, false } },
	{ 141, { false, false, true, true, true, false } },
	{ 142, { false, false, true, true, true, false } },
	{ 143, { false, false, true, true, true, false } },
	{ 144, { true, true, true, true, true, false } },
	{ 145, { true, true, true, true, true, false } },
	{ 146, { true, true, true, true, true, false } },
	{ 147, { true, true, true, true, true, false } },
	{ 148, { true, true, true, true, true, false } },
	{ 149, { true, true, true, true, true, false } },
	{ 150, { true, true, true, true, true, false } },
	{ 151, { true, true, true, true, true, false } },
	{ 152, { true, true, true, true, true, false } },
	{ 153, { true, true, true, true, true, false } },
	{ 154, { true, true, false, false, false, false } },
	{ 155, { true, true, false, false, false, false } },
	{ 156, { true, true, false, false, false, false } },
	{ 157, { true, false, false, false, false, false } },
	{ 158, { true, true, true, true, true, false } },
	{ 159, { true, true, true, true, true, false } },
	{ 160, { true, true, true, true, true, false } },
	{ 161, { false, true, true, true, true, false } },
	{ 162, { true, true, true, true, true, false } },
	{ 163, { false, true, true, true, true, false } },
	{ 164, { false, true, true, true, true, false } },
	{ 165, { false, true, true, true, true, false } },
	{ 166, { true, true, true, true, true, false } },
	{ 167, { true, false, true, false, false, false } },
	{ 168, { true, false, true, false, false, false } },
	{ 169, { true, false, true, false, false, false } },
	{ 170, { true, false, false, false, false, false } },
	{ 171, { true, false, false, false, false, false } },
	{ 172, { true, false, false, false, false, false } },
	{ 173, { true, false, false, false, false, false } },
	{ 174, { true, false, false, false, false, false } },
	{ 175, { true, false, false, false, false, false } },
	{ 176, { true, false, false, false, false, false } },
	{ 177, { true, false, false, false, false, false } },
	{ 178, { true, false, false, false, false, false } },
	{ 179, { true, false, false, false, false, false } },
	{ 180, { true, false, false, false, false, false } },
	{ 181, { true, false, false, false, false, false } },
	{ 182, { true, false, false, false, false, false } },
	{ 183, { true, false, false, false, false, false } },
	{ 184, { true, false, false, false, false, false } },
	{ 185, { true, false, false, false, false, false } },
	{ 186, { true, false, false, false, false, false } },
	{ 187, { true, false, false, false, false, false } },
	{ 188, { true, false, false, false, false, false } },
	{ 190, { true, true, true, true, true, false } },
	{ 191, { true, true, true, true, true, false } },
	{ 192, { true, true, true, true, true, false } },
	{ 193, { true, true, true, true, true, false } },
	{ 194, { true, true, true, true, true, false } },
	{ 195, { true, true, true, true, true, false } },
	{ 196, { true, true, true, true, true, false } },
	{ 197, { true, true, true, true, true, false } },
	{ 200, { true, true, true, true, true, false } },
	{ 201, { true, true, true, true, true, false } },
	{ 202, { true, true, true, true, true, false } },
	{ 203, { true, true, true, true, true, false } },
	{ 204, { true, true, true, true, true, false } },
	{ 205, { true, true, true, true, true, false } },
	{ 206, { true, true, true, true, true, false } },
	{ 207, { true, true, true, true, true, false } },
	{ 208, { true, true, true, true, true, false } },
	{ 209, { true, true, true, true, true, false } },
	{ 210, { true, true, true, true, true, false } },
	{ 211, { true, true, true, true, true, false } },
	{ 212, { true, true, true, true, true, false } },
	{ 213, { true, true, true, true, true, false } },
	{ 215, { true, true, true, true, true, false } },
	{ 216, { true, true, true, true, true, false } },
	{ 217, { true, true, true, true, true, false } },
	{ 218, { true, true, true, true, true, false } },
	{ 219, { true, true, true, true, true, false } },
	{ 220, { true, true, true, true, true, false } },
	{ 221, { true, true, true, false, true, false } },
	{ 222, { true, true, true, true, true, false } },
	{ 223, { true, true, true, true, true, false } },
	{ 224, { true, true, true, true, true, false } },
	{ 226, { false, false, false, true, false, false } },
	{ 228, { true, true, true, false, true, false } },
	{ 229, { true, true, true, false, true, false } },
	{ 230, { true, true, true, true, true, false } },
	{ 231, { true, true, true, true, true, false } },
	{ 232, { true, true, true, true, true, false } },
	{ 233, { true, true, true, true, true, false } },
	{ 234, { true, true, true, true, true, false } },
	{ 235, { true, true, true, true, true, false } },
	{ 236, { true, true, true, true, true, false } },
	{ 237, { true, true, true, true, true, false } },
	{ 238, { true, true, true, true, true, false } },
	{ 239, { true, true, true, true, true, false } },
	{ 240, { true, true, true, true, true, false } },
	{ 241, { true, false, false, true, false, false } },
	{ 242, { true, true, true, true, true, false } },
	{ 243, { true, false, false, false, false, false } },
	{ 244, { true, false, false, true, false, false } },
	{ 245, { true, false, false, false, false, false } },
	{ 246, { true, false, false, false, false, false } },
	{ 247, { true, true, true, false, true, false } },
	{ 248, { true, true, true, false, true, false } },
	{ 249, { true, false, false, true, false, false } },
	{ 250, { true, false, false, true, false, false } },
	{ 251, { true, false, false, false, false, false } },
	{ 253, { true, false, false, false, false, false } },
	{ 258, { true, false, false, false, false, false } },
	{ 259, { true, false, false, false, false, false } },
	{ 292, { true, true, true, true, true, false } },
	{ 294, { true, true, true, true, true, false } },
	{ 296, { true, true, true, true, true, false } },
	{ 300, { true, true, true, true, true, false } },
	{ 301, { true, true, true, true, true, false } },
	{ 302, { true, true, true, true, true, false } },
	{ 303, { true, true, true, true, true, false } },
	{ 304, { true, true, true, true, true, false } },
	{ 305, { true, true, true, true, true, false } },
	{ 306, { true, true, true, true, true, false } },
	{ 307, { true, true, true, true, true, false } },
	{ 308, { true, true, true, true, true, false } },
	{ 309, { true, true, true, true, true, false } },
	{ 310, { false, false, true, false, false, false } },
	{ 311, { false, false, true, false, false, false } },
	{ 312, { false, false, false, false, true, false } },
	{ 313, { false, false, false, false, true, false } },
	{ 314, { true, false, false, false, false, false } },
	{ 315, { true, false, false, false, false, false } },
	{ 316, { true, false, false, false, false, false } },
	{ 317, { true, false, false, false, false, false } },
	{ 318, { false, true, false, false, false, false } },
	{ 319, { false, true, false, false, false, false } },
	{ 320, { true, false, false, false, false, false } },
	{ 321, { true, false, false, false, false, false } },
	{ 322, { false, true, false, false, false, false } },
	{ 323, { false, true, false, false, false, false } },
	{ 324, { true, true, true, true, true, false } },
	{ 325, { true, true, true, true, true, false } },
	{ 326, { true, true, true, true, true, false } },
	{ 327, { true, true, true, true, true, false } },
	{ 328, { true, true, true, true, true, false } },
	{ 329, { true, true, true, true, true, false } },
	{ 330, { true, true, true, true, true, false } },
	{ 331, { true, true, true, true, true, false } },
	{ 332, { true, true, true, true, true, false } },
	{ 333, { true, true, true, true, true, false } },
	{ 334, { true, true, true, true, true, false } },
	{ 335, { true, true, true, true, true, false } },
	{ 336, { true, true, true, true, true, false } },
	{ 337, { true, true, true, true, true, false } },
	{ 338, { true, true, true, true, true, false } },
	{ 339, { true, true, true, true, true, false } },
	{ 340, { true, true, true, true, true, false } },
	{ 341, { true, true, true, true, true, false } },
	{ 342, { true, true, true, true, true, false } },
	{ 343, { true, true, true, true, true, false } },
	{ 344, { true, true, true, true, true, false } },
	{ 345, { true, true, true, true, true, false } },
	{ 346, { true, true, true, true, true, false } },
	{ 347, { true, true, true, true, true, false } },
	{ 348, { true, true, true, true, true, false } },
	{ 349, { true, true, true, true, true, false } },
	{ 350, { true, true, true, true, true, false } },
	{ 351, { true, true, true, true, true, false } },
	{ 352, { true, true, true, true, true, false } },
	{ 353, { true, true, true, true, true, false } },
	{ 354, { true, true, true, true, true, false } },
	{ 355, { true, true, true, true, true, false } },
	{ 356, { true, true, true, true, true, false } },
	{ 357, { true, true, true, true, true, false } },
	{ 358, { true, true, true, true, true, false } },
	{ 359, { true, true, true, true, true, false } },
	{ 360, { true, true, true, true, true, false } },
	{ 361, { true, true, true, true, true, false } },
	{ 362, { true, true, true, true, true, false } },
	{ 363, { true, true, true, true, true, false } },
	{ 364, { true, true, true, true, true, false } },
	{ 365, { true, true, true, true, true, false } },
	{ 366, { true, true, true, true, true, false } },
	{ 367, { true, true, true, true, true, false } },
	{ 368, { true, true, true, true, true, false } },
	{ 369, { true, true, true, true, true, false } },
	{ 370, { true, true, true, true, true, false } },
	{ 371, { true, true, true, true, true, false } },
	{ 372, { false, false, true, false, false, false } },
	{ 373, { false, false, false, false, true, false } },
	{ 374, { true, false, false, false, false, false } },
	{ 375, { true, false, false, false, false, false } },
	{ 376, { true, false, false, false, false, false } },
	{ 377, { true, false, false, false, false, false } },
	{ 378, { true, true, true, true, true, false } },
	{ 379, { true, true, true, true, true, false } },
	{ 380, { true, true, true, true, true, false } },
	{ 381, { true, true, true, true, true, false } },
	{ 382, { true, true, true, true, true, false } },
	{ 383, { true, true, true, true, true, false } },
	{ 384, { true, true, true, true, true, false } },
	{ 385, { true, true, true, true, true, false } },
	{ 386, { true, true, true, true, true, false } },
	{ 387, { true, true, true, true, true, false } },
	{ 388, { true, true, true, true, true, false } },
	{ 389, { true, true, true, true, true, false } },
	{ 390, { true, true, true, true, true, false } },
	{ 391, { true, true, true, true, true, false } },
	{ 392, { true, true, true, true, true, false } },
	{ 393, { true, true, true, true, true, false } },
	{ 394, { true, true, true, true, true, false } },
	{ 395, { true, true, true, true, true, false } },
	{ 396, { true, true, true, true, true, false } },
	{ 397, { true, true, true, true, true, false } },
	{ 398, { true, true, true, true, true, false } },
	{ 399, { true, true, true, true, true, false } },
	{ 400, { true, true, true, true, true, false } },
	{ 401, { true, true, true, true, true, false } },
	{ 402, { true, true, true, true, true, false } },
	{ 403, { true, true, true, true, true, false } },
	{ 404, { true, true, true, true, true, false } },
	{ 405, { true, true, true, true, true, false } },
	{ 406, { true, true, true, true, true, false } },
	{ 407, { true, true, true, true, true, false } },
	{ 408, { true, false, false, false, false, false } },
	{ 409, { true, false, false, false, false, false } },
	{ 410, { true, false, false, false, false, false } },
	{ 411, { true, true, true, true, true, false } },
	{ 412, { true, true, true, true, true, false } },
	{ 413, { true, true, true, true, true, false } },
	{ 414, { true, true, true, true, true, false } },
	{ 415, { true, true, true, true, true, false } },
	{ 416, { true, true, true, true, true, false } },
	{ 417, { true, true, true, true, true, false } },
	{ 418, { true, true, true, true, true, false } },
	{ 419, { true, true, true, true, true, false } },
	{ 420, { true, true, true, true, true, false } },
	{ 421, { true, true, true, true, true, false } },
	{ 422, { true, true, true, true, true, false } },
	{ 423, { true, true, true, true, true, false } },
	{ 424, { true, true, true, true, true, false } },
	{ 425, { true, false, false, false, false, false } },
	{ 426, { true, true, true, true, true, false } },
	{ 427, { true, true, true, true, true, false } },
	{ 428, { true, true, true, true, true, false } },
	{ 429, { true, true, true, true, true, false } },
	{ 430, { true, true, true, true, true, false } },
	{ 431, { true, true, true, true, true, false } },
	{ 432, { true, true, true, true, true, false } },
	{ 433, { true, true, true, true, true, false } },
	{ 434, { true, true, true, true, true, false } },
	{ 435, { true, true, true, true, true, false } },
	{ 436, { true, true, true, true, true, false } },
	{ 437, { true, true, true, true, true, false } },
	{ 438, { true, true, true, true, true, false } },
	{ 439, { true, true, true, true, true, false } },
	{ 440, { true, true, true, true, true, false } },
	{ 441, { true, true, true, true, true, false } },
	{ 442, { true, true, true, true, true, false } },
	{ 443, { true, true, true, true, true, false } },
	{ 444, { true, true, true, true, true, false } },
	{ 445, { true, true, true, true, true, false } },
	{ 446, { true, true, true, true, true, false } },
	{ 447, { true, true, true, true, true, false } },
	{ 448, { true, true, true, true, true, false } },
	{ 449, { true, true, true, true, true, false } },
	{ 450, { true, true, true, true, true, false } },
	{ 451, { true, true, true, true, true, false } },
	{ 452, { true, false, false, false, false, false } },
	{ 453, { true, true, true, true, true, false } },
	{ 454, { true, true, true, true, true, false } },
	{ 455, { true, true, true, true, true, false } },
	{ 456, { true, true, true, true, true, false } },
	{ 457, { true, true, true, true, true, false } },
	{ 458, { true, true, true, true, true, false } },
	{ 459, { true, true, true, true, true, false } },
	{ 460, { true, true, true, true, true, false } },
	{ 461, { true, true, true, true, true, false } },
	{ 462, { true, true, true, true, true, false } },
	{ 463, { true, false, false, false, false, false } },
	{ 464, { true, true, true, true, true, false } },
	{ 465, { true, true, true, true, true, false } },
	{ 466, { true, true, true, true, true, false } },
	{ 467, { true, true, true, true, true, false } },
	{ 468, { true, true, true, true, true, false } },
	{ 469, { true, true, true, true, true, false } },
	{ 470, { true, true, true, true, true, false } },
	{ 471, { true, true, true, true, true, false } },
	{ 472, { true, true, true, true, true, false } },
	{ 473, { true, true, true, true, true, false } },
	{ 474, { true, true, true, true, true, false } },
	{ 475, { true, true, true, true, true, false } },
	{ 476, { true, true, true, true, true, false } },
	{ 477, { true, true, true, true, true, false } },
	{ 478, { true, true, true, true, true, false } },
	{ 479, { true, true, true, true, true, false } },
	{ 480, { true, true, true, true, true, false } },
	{ 481, { true, true, true, true, true, false } },
	{ 482, { true, true, true, true, true, false } },
	{ 483, { true, true, true, true, true, false } },
	{ 484, { true, true, true, true, true, false } },
	{ 485, { true, true, true, true, true, false } },
	{ 486, { true, true, true, true, true, false } },
	{ 487, { true, true, true, true, true, false } },
	{ 488, { true, false, false, false, false, false } },
	{ 489, { true, false, false, false, false, false } },
	{ 490, { true, false, false, false, false, false } },
	{ 491, { true, false, false, false, false, false } },
	{ 492, { true, true, true, true, true, false } },
	{ 493, { true, true, true, true, true, false } },
	{ 494, { true, true, true, true, true, false } },
	{ 495, { true, true, true, true, true, false } },
	{ 496, { true, true, true, true, true, false } },
	{ 497, { true, true, true, true, true, false } },
	{ 498, { true, true, true, true, true, false } },
	{ 499, { true, true, true, true, true, false } },
	{ 500, { true, true, true, true, true, false } },
	{ 501, { true, true, true, true, true, false } },
	{ 502, { true, true, true, true, true, false } },
	{ 503, { true, true, true, true, true, false } },
	{ 504, { true, true, true, true, true, false } },
	{ 505, { true, true, true, true, true, false } },
	{ 506, { true, true, true, true, true, false } },
	{ 507, { true, true, true, true, true, false } },
	{ 508, { true, true, true, true, true, false } },
	{ 509, { true, true, true, true, true, false } },
	{ 510, { true, true, true, true, true, false } },
	{ 511, { true, true, true, true, true, false } },
	{ 512, { true, true, true, true, true, false } },
	{ 513, { true, false, false, false, false, false } },
	{ 514, { true, false, false, false, false, false } },
	{ 515, { true, false, false, false, false, false } },
	{ 516, { true, false, false, false, false, false } },
	{ 517, { false, true, true, true, true, false } },
	{ 518, { true, true, true, true, true, false } },
	{ 519, { true, true, true, true, true, false } },
	{ 520, { true, true, true, true, true, false } },
	{ 521, { true, true, true, true, true, false } },
	{ 522, { true, true, true, true, true, false } },
	{ 524, { true, true, true, true, true, false } },
	{ 525, { true, true, true, true, true, false } },
	{ 526, { true, true, true, true, true, false } },
	{ 527, { true, true, true, true, true, false } },
	{ 528, { true, true, true, true, true, false } },
	{ 529, { true, true, true, true, true, false } },
	{ 530, { true, true, true, true, true, false } },
	{ 531, { true, true, true, true, true, false } },
	{ 535, { true, true, true, true, true, false } },
	{ 537, { true, true, true, true, true, false } },
	{ 538, { true, false, false, false, false, false } },
	{ 539, { false, true, false, false, false, false } },
	{ 540, { true, true, true, true, true, false } },
	{ 541, { true, true, true, true, true, false } },
	{ 542, { true, true, true, true, true, false } },
	{ 543, { true, true, true, true, true, false } },
	{ 544, { true, true, true, true, true, false } },
	{ 545, { true, true, true, true, true, false } },
	{ 546, { true, true, true, true, true, false } },
	{ 547, { true, true, true, true, true, false } },
	{ 548, { true, true, true, true, true, false } },
	{ 549, { true, true, true, true, true, false } },
	{ 550, { true, true, true, true, true, false } },
	{ 551, { true, true, true, true, true, false } },
	{ 552, { true, true, true, true, true, false } },
	{ 553, { true, true, true, true, true, false } },
	{ 554, { true, true, true, true, true, false } },
	{ 555, { true, true, true, true, true, false } },
	{ 556, { true, true, true, true, true, false } },
	{ 557, { true, true, true, true, true, false } },
	{ 558, { true, true, true, true, true, false } },
	{ 559, { true, true, true, true, true, false } },
	{ 560, { true, true, true, true, true, false } },
	{ 561, { true, true, true, true, true, false } },
	{ 562, { true, true, true, true, true, false } },
	{ 563, { true, true, true, true, true, false } },
	{ 564, { true, true, true, true, true, false } },
	{ 565, { true, true, true, true, true, false } },
	{ 566, { true, true, true, true, true, false } },
	{ 567, { true, true, true, true, true, false } },
	{ 568, { true, true, true, true, true, false } },
	{ 569, { true, true, true, true, true, false } },
	{ 570, { true, true, true, true, true, false } },
	{ 571, { true, true, true, true, true, false } },
	{ 572, { true, true, true, true, true, false } },
	{ 573, { true, true, true, true, true, false } },
	{ 574, { true, true, true, true, true, false } },
	{ 575, { true, true, true, true, true, false } },
	{ 576, { true, true, true, true, true, false } },
	{ 577, { true, true, true, true, true, false } },
	{ 578, { true, true, true, true, true, false } },
	{ 579, { true, true, true, true, true, false } },
	{ 580, { true, true, true, true, true, false } },
	{ 581, { true, true, true, true, true, false } },
	{ 582, { true, true, true, true, true, false } },
	{ 583, { true, true, true, true, true, false } },
	{ 584, { true, true, true, true, true, false } },
	{ 585, { true, true, true, true, true, false } },
	{ 586, { true, true, true, true, true, false } },
	{ 587, { true, true, true, true, true, false } },
	{ 588, { true, true, true, true, true, false } },
	{ 589, { true, true, true, true, true, false } },
	{ 590, { true, true, true, true, true, false } },
	{ 591, { true, true, true, true, true, false } },
	{ 592, { true, true, true, true, true, false } },
	{ 593, { true, true, true, true, true, false } },
	{ 594, { true, true, true, true, true, false } },
	{ 595, { true, true, true, true, true, false } },
	{ 596, { true, true, true, true, true, false } },
	{ 597, { true, false, false, false, false, false } },
	{ 598, { false, true, false, false, false, false } },
	{ 599, { true, true, true, true, true, false } },
	{ 600, { true, true, true, true, true, false } },
	{ 601, { true, true, true, true, true, false } },
	{ 602, { true, true, true, true, true, false } },
	{ 603, { true, true, true, true, true, false } },
	{ 604, { true, true, true, true, true, false } },
	{ 605, { true, true, true, true, true, false } },
	{ 606, { true, true, true, true, true, false } },
	{ 607, { true, true, true, true, true, false } },
	{ 608, { true, true, true, true, true, false } },
	{ 609, { true, true, true, true, true, false } },
	{ 610, { true, true, true, true, true, false } },
	{ 611, { true, true, true, true, true, false } },
	{ 612, { true, true, true, true, true, false } },
	{ 613, { true, true, true, true, true, false } },
	{ 614, { true, true, true, true, true, false } },
	{ 615, { true, true, true, true, true, false } },
	{ 616, { true, true, true, true, true, false } },
	{ 617, { true, true, true, true, true, false } },
	{ 618, { true, true, true, true, true, false } },
	{ 619, { true, true, true, true, true, false } },
	{ 620, { true, true, true, true, true, false } },
	{ 621, { true, true, true, true, true, false } },
	{ 622, { true, true, true, true, true, false } },
	{ 623, { true, true, true, true, true, false } },
	{ 624, { true, true, true, true, true, false } },
	{ 625, { true, true, true, true, true, false } },
	{ 626, { true, true, true, true, true, false } },
	{ 627, { true, true, true, true, true, false } },
	{ 628, { true, true, true, true, true, false } },
	{ 629, { true, true, true, true, true, false } },
	{ 630, { true, true, true, true, true, false } },
	{ 631, { true, true, true, true, true, false } },
	{ 632, { true, true, true, true, true, false } },
	{ 633, { true, true, true, true, true, false } },
	{ 634, { true, true, true, true, true, false } },
	{ 635, { true, true, true, true, true, false } },
	{ 636, { true, true, true, true, true, false } },
	{ 637, { true, true, true, true, true, false } },
	{ 638, { true, true, true, true, true, false } },
	{ 639, { true, true, true, true, true, false } },
	{ 640, { true, true, true, true, true, false } },
	{ 641, { true, true, true, true, true, false } },
	{ 642, { true, true, true, true, true, false } },
	{ 643, { true, true, true, true, true, false } },
	{ 644, { true, true, true, true, true, false } },
	{ 645, { true, true, true, true, true, false } },
	{ 646, { true, true, true, true, true, false } },
	{ 647, { true, true, true, true, true, false } },
	{ 648, { true, true, true, true, true, false } },
	{ 649, { true, true, true, true, true, false } },
	{ 650, { true, true, true, true, true, false } },
	{ 651, { true, true, true, true, true, false } },
	{ 652, { true, true, true, true, true, false } },
	{ 653, { true, true, true, true, true, false } },
	{ 654, { true, true, true, true, true, false } },
	{ 655, { true, true, true, true, true, false } },
	{ 656, { true, true, true, true, true, false } },
	{ 657, { true, true, true, true, true, false } },
	{ 658, { true, true, true, true, true, false } },
	{ 659, { true, true, true, true, true, false } },
	{ 660, { true, true, true, true, true, false } },
	{ 661, { true, true, true, true, true, false } },
	{ 662, { true, true, true, true, true, false } },
	{ 663, { true, true, true, true, true, false } },
	{ 664, { true, true, true, true, true, false } },
	{ 665, { true, true, true, true, true, false } },
	{ 666, { true, true, true, true, true, false } },
	{ 667, { true, true, true, true, true, false } },
	{ 668, { true, true, true, true, true, false } },
	{ 669, { true, true, true, true, true, false } },
	{ 670, { true, true, true, true, true, false } },
	{ 671, { true, true, true, true, true, false } },
	{ 672, { true, true, true, true, true, false } },
	{ 673, { true, true, true, true, true, false } },
	{ 674, { true, true, true, true, true, false } },
	{ 675, { true, true, true, true, true, false } },
	{ 676, { true, false, false, false, false, false } },
	{ 677, { true, false, false, false, false, false } },
	{ 678, { true, true, true, true, true, false } },
	{ 679, { true, true, true, true, true, false } },
	{ 680, { true, true, true, true, true, false } },
	{ 681, { true, true, true, true, true, false } },
	{ 682, { true, true, true, true, true, false } },
	{ 683, { true, true, true, true, true, false } },
	{ 684, { true, true, true, true, true, false } },
	{ 685, { true, true, true, true, true, false } },
	{ 686, { true, true, true, true, true, false } },
	{ 687, { true, true, true, true, true, false } },
	{ 688, { true, true, true, true, true, false } },
	{ 689, { true, true, true, true, true, false } },
	{ 690, { true, true, true, true, true, false } },
	{ 691, { true, true, true, true, true, false } },
	{ 692, { true, true, true, true, true, false } },
	{ 693, { true, true, true, true, true, false } },
	{ 694, { true, true, true, true, true, false } },
	{ 695, { true, true, true, true, true, false } },
	{ 696, { true, true, true, true, true, false } },
	{ 697, { true, true, true, true, true, false } },
	{ 698, { true, true, true, true, true, false } },
	{ 699, { true, true, true, true, true, false } },
	{ 700, { true, true, true, true, true, false } },
	{ 701, { true, true, true, true, true, false } },
	{ 702, { true, true, true, true, true, false } },
	{ 703, { true, true, true, true, true, false } },
	{ 704, { true, true, true, true, true, false } },
	{ 705, { true, true, true, true, true, false } },
	{ 706, { true, true, true, true, true, false } },
	{ 707, { true, true, true, true, true, false } },
	{ 708, { true, true, true, true, true, false } },
	{ 709, { true, true, true, true, true, false } },
	{ 710, { true, true, true, true, true, false } },
	{ 711, { true, false, false, false, false, false } },
	{ 712, { true, false, false, false, false, false } },
	{ 713, { true, false, false, false, false, false } },
	{ 714, { true, false, false, false, false, false } },
	{ 715, { false, true, true, true, true, false } },
	{ 716, { true, true, true, true, true, false } },
	{ 719, { true, true, true, true, true, false } },
	{ 720, { true, true, true, true, true, false } },
	{ 722, { false, true, true, true, true, false } },
	{ 723, { false, true, true, true, true, false } },
	{ 724, { false, true, true, true, true, false } },
	{ 725, { true, true, true, true, true, false } },
	{ 726, { true, true, true, true, true, false } },
	{ 727, { true, true, true, true, true, false } },
};