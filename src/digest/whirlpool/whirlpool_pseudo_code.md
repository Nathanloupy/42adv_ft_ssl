// Constants
const DIGEST_SIZE = 512 // bits
const DIGEST_SIZE_BYTES = 64 // bytes
const BLOCK_SIZE = 512 // bits
const BLOCK_SIZE_BYTES = 64 // bytes
const NUM_ROUNDS = 10

// S-box (16×16 matrix of bytes)
const byte[16][16] SBOX = {
    {0x18, 0x23, 0xC6, 0xE8, 0x87, 0xB8, 0x01, 0x4F, 0x36, 0xA6, 0xD2, 0xF5, 0x79, 0x6F, 0x91, 0x52},
    {0x60, 0xBC, 0x9B, 0x8E, 0xA3, 0x0C, 0x7B, 0x35, 0x1D, 0xE0, 0xD7, 0xC2, 0x2E, 0x4B, 0xFE, 0x57},
    // ... remaining rows from Table 2(a) in the document
}

// C matrix (8×8 circulant MDS matrix)
const byte[8][8] C_MATRIX = {
    {0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09},
    {0x09, 0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02},
    {0x02, 0x09, 0x01, 0x01, 0x04, 0x01, 0x08, 0x05},
    {0x05, 0x02, 0x09, 0x01, 0x01, 0x04, 0x01, 0x08},
    {0x08, 0x05, 0x02, 0x09, 0x01, 0x01, 0x04, 0x01},
    {0x01, 0x08, 0x05, 0x02, 0x09, 0x01, 0x01, 0x04},
    {0x04, 0x01, 0x08, 0x05, 0x02, 0x09, 0x01, 0x01},
    {0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09, 0x01}
}

// Irreducible polynomial for GF(2^8)
const int POLYNOMIAL = 0x11D // x^8 + x^4 + x^3 + x^2 + 1

// Round constants (calculated based on S-box)
byte[10][8][8] RC

// Main Whirlpool hash function
function byte[64] Whirlpool(byte[] message):
    // Step 1 & 2: Pad message and append length
    byte[] paddedMessage = PadMessage(message)
    
    // Step 3: Initialize hash matrix
    byte[8][8] hashMatrix = {{0}} // All zeros
    
    // Step 4: Process message in blocks
    for i = 0 to paddedMessage.length/64 - 1:
        byte[8][8] block = ConvertToMatrix(paddedMessage, i * 64)
        hashMatrix = ProcessBlock(hashMatrix, block)
    
    // Convert final matrix back to linear array
    return MatrixToArray(hashMatrix)

// Pad message according to Whirlpool specifications
function byte[] PadMessage(byte[] message):
    // Calculate message length in bits
    long messageLengthBits = message.length * 8
    
    // Calculate number of padding bytes needed
    // Ensure length is odd multiple of 256 bits + 256 bits for length
    int paddingLength = 32 - (message.length % 32)
    if paddingLength == 0:
        paddingLength = 32
    
    // Create new padded message
    byte[] result = new byte[message.length + paddingLength + 32]
    
    // Copy original message
    for i = 0 to message.length - 1:
        result[i] = message[i]
    
    // Add padding: 1 bit followed by 0s
    result[message.length] = 0x80 // 10000000
    for i = message.length + 1 to message.length + paddingLength - 1:
        result[i] = 0x00
    
    // Append original message length as 256-bit value (32 bytes)
    // Store in big-endian (most significant byte first)
    int lengthPos = message.length + paddingLength
    for i = 0 to 31:
        // Set the last 32 bytes with the message length
        if i < 24:
            result[lengthPos + i] = 0 // Higher bytes are 0 for typical messages
        else:
            // Extract appropriate byte from messageLengthBits
            int shift = (31 - i) * 8
            result[lengthPos + i] = (byte)(messageLengthBits >> shift)
    
    return result

// Convert 64 bytes starting at offset into 8×8 matrix
function byte[8][8] ConvertToMatrix(byte[] data, int offset):
    byte[8][8] matrix
    for i = 0 to 7:
        for j = 0 to 7:
            matrix[i][j] = data[offset + 8*i + j]
    return matrix

// Convert 8×8 matrix back to 64-byte array
function byte[64] MatrixToArray(byte[8][8] matrix):
    byte[64] result
    for i = 0 to 7:
        for j = 0 to 7:
            result[8*i + j] = matrix[i][j]
    return result

// Process a single 512-bit block
function byte[8][8] ProcessBlock(byte[8][8] hashValue, byte[8][8] messageBlock):
    // Initialize the cipher key and state
    byte[8][8] key = hashValue
    byte[8][8] state = messageBlock
    
    // Initialize round keys
    byte[11][8][8] roundKeys = ExpandKey(key)
    
    // Initial AddKey
    state = AddKey(state, roundKeys[0])
    
    // 10 rounds of encryption
    for r = 1 to 10:
        state = SubBytes(state)
        state = ShiftColumns(state)
        state = MixRows(state)
        state = AddKey(state, roundKeys[r])
    
    // XOR with original hash value and message block
    byte[8][8] result
    for i = 0 to 7:
        for j = 0 to 7:
            result[i][j] = state[i][j] ^ hashValue[i][j] ^ messageBlock[i][j]
    
    return result

// SubBytes transformation
function byte[8][8] SubBytes(byte[8][8] state):
    byte[8][8] result
    for i = 0 to 7:
        for j = 0 to 7:
            // Get row and column indices for S-box
            byte row = (state[i][j] >> 4) & 0x0F
            byte col = state[i][j] & 0x0F
            result[i][j] = SBOX[row][col]
    return result

// ShiftColumns transformation
function byte[8][8] ShiftColumns(byte[8][8] state):
    byte[8][8] result
    for i = 0 to 7:
        for j = 0 to 7:
            // Calculate new row index with appropriate shift
            // j=0: no shift, j=1: shift by 1, j=2: shift by 2, etc.
            int newRow = (i + j) % 8
            result[newRow][j] = state[i][j]
    return result

// MixRows transformation
function byte[8][8] MixRows(byte[8][8] state):
    byte[8][8] result
    
    for i = 0 to 7:
        for j = 0 to 7:
            result[i][j] = 0
            for k = 0 to 7:
                // Multiply in GF(2^8)
                result[i][j] ^= GFMultiply(C_MATRIX[i][k], state[i][k])
    
    return result

// AddKey transformation
function byte[8][8] AddKey(byte[8][8] state, byte[8][8] key):
    byte[8][8] result
    for i = 0 to 7:
        for j = 0 to 7:
            result[i][j] = state[i][j] ^ key[i][j]
    return result

// Key expansion
function byte[11][8][8] ExpandKey(byte[8][8] key):
    // Initialize round constants
    InitializeRoundConstants()
    
    // Key schedule
    byte[11][8][8] roundKeys
    roundKeys[0] = key
    
    for r = 1 to 10:
        byte[8][8] K = roundKeys[r-1]
        
        // Apply round function using round constant
        K = SubBytes(K)
        K = ShiftColumns(K)
        K = MixRows(K)
        K = AddKey(K, RC[r-1])
        
        roundKeys[r] = K
    
    return roundKeys

// Initialize round constants
function InitializeRoundConstants():
    // Initialize RC arrays
    for r = 1 to 10:
        for i = 0 to 7:
            for j = 0 to 7:
                RC[r-1][i][j] = 0
        
        // Set first row with S-box values
        for j = 0 to 7:
            byte index = 8 * (r - 1) + j
            byte row = (index >> 4) & 0x0F
            byte col = index & 0x0F
            RC[r-1][0][j] = SBOX[row][col]

// Multiply two bytes in GF(2^8)
function byte GFMultiply(byte a, byte b):
    byte p = 0
    byte highBit
    
    for i = 0 to 7:
        if (b & 1) == 1:
            p ^= a
        
        highBit = (a & 0x80)
        a <<= 1
        
        if highBit == 0x80:
            a ^= POLYNOMIAL & 0xFF
        
        b >>= 1
    
    return p