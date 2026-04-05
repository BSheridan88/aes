# AES-256 File Encryption — C Implementation

A from-scratch implementation of AES-256 (Advanced Encryption Standard) written in C. Built as a portfolio project to demonstrate deep understanding of symmetric cryptography, Galois Field arithmetic, and low-level systems programming.

---

## Overview

This tool encrypts and decrypts files using a hand-rolled AES-256 cipher with 14 rounds of the full AES transformation pipeline. No high-level crypto libraries are used for the cipher itself — every component (SubBytes, ShiftRows, MixColumns, AddRoundKey) is implemented manually. [libsodium](https://libsodium.org) is used solely for cryptographically secure random salt generation during key derivation.

---

## Features

- **Full AES-256 pipeline** — SubBytes, ShiftRows, MixColumns, AddRoundKey across 14 rounds
- **Inverse cipher** — Complete decryption path with InvSubBytes, InvShiftRows, InvMixColumns
- **Galois Field (GF 2⁸) arithmetic** — Manual GF multiplication used in MixColumns/InvMixColumns
- **Hand-coded S-Box & Inverse S-Box** — Full 256-entry lookup tables as per the AES specification
- **Key derivation with random salt** — Pads user keys to 32 bytes using a libsodium-generated salt
- **Automatic key file management** — Saves the derived key to a `.key` file on encrypt, loads it automatically on decrypt
- **XOR layer** — Additional XOR pass over data using the derived key each round

---

## How It Works

### Encryption
1. File is read into a buffer
2. User provides a passphrase (any length)
3. Key is derived: passphrase bytes + random salt padded to 32 bytes, then expanded to 240 bytes for 15 round keys
4. Initial `AddRoundKey` (round 0)
5. Rounds 1–13: `XOR` → `SubBytes` → `ShiftRows` → `MixColumns` → `AddRoundKey`
6. Final round 14: `XOR` → `SubBytes` → `ShiftRows` → `AddRoundKey` (no MixColumns)
7. Ciphertext written to `.enc` output file; derived key saved to `.enc.key`

### Decryption
1. Encrypted file and matching `.key` file are loaded
2. Round keys applied in reverse order
3. Inverse transformations applied: `AddRoundKey` → `InvShiftRows` → `InvSubBytes` → `XOR`, with `InvMixColumns` for rounds 1–13
4. Plaintext written to `.dec` output file

---

## Dependencies

| Library | Purpose |
|---|---|
| [libsodium](https://libsodium.org) | Cryptographically secure random bytes (`randombytes_buf`) for salt generation |

Install libsodium on Debian/Ubuntu:
```bash
sudo apt install libsodium-dev
```

On macOS with Homebrew:
```bash
brew install libsodium
```

---

## Build
```bash
gcc aes256.c -o aes256 -lsodium
```

---

## Usage

Run the binary:
```bash
./aes256
```

**Encrypt a file:**
```
What is the name of the file you would like to encrypt: secret.txt
Type E if you want to encrypt and D to decrypt: E
What is the key you would like to use: mypassphrase
What is the name of the exported file: secret.enc
```
Outputs: `secret.enc` and `secret.enc.key`

**Decrypt a file:**
```
What is the name of the file you would like to encrypt: secret.enc
Type E if you want to encrypt and D to decrypt: D
What is the name of the encrypted file: secret.enc
```
Outputs: `secret.enc.dec`

> ⚠️ Keep the `.key` file alongside the `.enc` file. Decryption is not possible without it.

---

## Implementation Notes

- Files are read into a fixed 300-byte buffer — suited for small text files and demonstration purposes
- The state array is a flat `unsigned char` buffer mapped to a 4×4 column-major grid matching the AES spec
- GF(2⁸) multiplication uses the irreducible polynomial `x⁸ + x⁴ + x³ + x + 1` (0x1B reduction)
- Round keys are sliced sequentially from the 240-byte `final_key` array (16 bytes × 15 rounds)

---

## Project Goals

This project was built to:
- Demonstrate understanding of the AES-256 specification at the byte level
- Practice low-level C programming: pointer arithmetic, file I/O, bitwise operations
- Explore finite field arithmetic (GF 2⁸) as applied in real-world cryptography
- Serve as a portfolio piece for cybersecurity roles

---

## Author

Built from scratch as a cybersecurity portfolio project. All AES transformations implemented manually without reference to crypto library internals.
