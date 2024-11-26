#pragma once

uint64_t test_text_to_uint64(const char* text);

void test_generate_keys(uint64_t subkeys[16]);

uint32_t test_des_function(uint32_t right, uint64_t subkey);

uint64_t test_round(uint64_t binary, uint64_t subkey, int count, int flag);