import hashlib
import random

def proof(byte_array):
    first_24_bits = (byte_array[0] << 16) | (byte_array[1] << 8) | byte_array[2]
    return first_24_bits == 0

def reverse_endianess(byte_array):
    return byte_array[::-1]

def print_bytes(byte_array):
    for b in byte_array:
        print(format(b, '02X'), end='')
    print()

def string_to_byte_array(hex_string):
    length = len(hex_string)
    byte_array = bytearray(length // 2)

    for i in range(0, length, 2):
        byte_array[i // 2] = int(hex_string[i:i+2], 16)

    return byte_array

def get_block_parameters():
    quote = input("Quote: ")
    prev_hash = input("Prev Hash: ")
    diff = 24
    print()
    return quote, prev_hash, diff

def generate_nonce():
    thirty_bits = random.randint(0, (1 << 30) - 1)
    two_bits = random.randint(0, (1 << 2) - 1)
    full_range = (thirty_bits << 2) | two_bits
    print("Nonce (int):", full_range)
    return full_range

def compute_block(quote, prev_hash, diff):
    proved = False
    quote_bytes = quote.encode('ascii')
    prev_hash_bytes = string_to_byte_array(prev_hash)
    print_bytes(prev_hash_bytes)
    print_bytes(quote_bytes)

    while not proved:
        nonce = generate_nonce()
        nonce_bytes = nonce.to_bytes(4, byteorder='little')
        nonce_bytes = reverse_endianess(nonce_bytes)
        cat_bytes = prev_hash_bytes + nonce_bytes + quote_bytes
        hash_result = hashlib.sha256(cat_bytes).digest()
        print_bytes(hash_result)
        proved = proof(hash_result)
        if proved:
            return hash_result
        
    return b'aaaaaaaaaa'

if __name__ == "__main__":
    quote, prev_hash, diff = get_block_parameters()
    compute_block(quote, prev_hash, diff)