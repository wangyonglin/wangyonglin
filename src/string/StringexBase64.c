#include <StringexBase64.h>

char *StringexBase64Encrypt(Stringex *callStringex,const void *b64_encode_this, int encode_this_many_bytes, Boolean with_new_line)
{

    BIO *b64_bio, *mem_bio;            // Declares two OpenSSL BIOs: a base64 filter and a memory BIO.
    BUF_MEM *mem_bio_mem_ptr;          // Pointer to a "memory BIO" structure holding our base64 data.
    b64_bio = BIO_new(BIO_f_base64()); // Initialize our base64 filter BIO.
    mem_bio = BIO_new(BIO_s_mem());    // Initialize our memory sink BIO.
    BIO_push(b64_bio, mem_bio);        // Link the BIOs by creating a filter-sink BIO chain.
    if (!with_new_line)
        BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);           // No newlines every 64 characters or less.
    BIO_write(b64_bio, b64_encode_this, encode_this_many_bytes);  // Records base64 encoded data.
    BIO_flush(b64_bio);                                           // Flush data.  Necessary for b64 encoding, because of pad characters.
    BIO_get_mem_ptr(mem_bio, &mem_bio_mem_ptr);                   // Store address of mem_bio's memory structure.
    BIO_set_close(mem_bio, BIO_NOCLOSE);                          // Permit access to mem_ptr after BIOs are destroyed.
    BIO_free_all(b64_bio);                                        // Destroys all BIOs in chain, starting with b64 (i.e. the 1st one).
    BUF_MEM_grow(mem_bio_mem_ptr, (*mem_bio_mem_ptr).length + 1); // Makes space for end null.
    (*mem_bio_mem_ptr).data[(*mem_bio_mem_ptr).length] = '\0';    // Adds null-terminator to tail.
	StringexCreate(callStringex,(*mem_bio_mem_ptr).data,(*mem_bio_mem_ptr).length);
	BUF_MEM_free(mem_bio_mem_ptr);
    return callStringex->valuestring;                               // Returns base-64 encoded data. (See: "buf_mem_st" struct).
}

char *StringexBase64Decrypt(Stringex *callStringex, const void *b64_decode_this, int decode_this_many_bytes, Boolean with_new_line)
{
	BIO *b64_bio, *mem_bio;
	callStringex->valuestring = calloc((decode_this_many_bytes * 3) / 4 + 1, sizeof(char));
	b64_bio = BIO_new(BIO_f_base64());
	mem_bio = BIO_new(BIO_s_mem());
	BIO_write(mem_bio, b64_decode_this, decode_this_many_bytes);
	BIO_push(b64_bio, mem_bio);
	if (!with_new_line)
		BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL); // Don't require trailing newlines.
	callStringex->valuelength = 0;						// Index where the next base64_decoded byte should be written.
	while (0 < BIO_read(b64_bio, callStringex->valuestring + callStringex->valuelength, 1))
	{								 // Read byte-by-byte.
		callStringex->valuelength++; // Increment the index until read of BIO decoded data is complete.
	}
	// Once we're done reading decoded data, BIO_read returns -1 even though there's no error.
	BIO_free_all(b64_bio);			  // Destroys all BIOs in chain, starting with b64 (i.e. the 1st one).
	return callStringex->valuestring; // Returns base-64 decoded data with trailing null terminator.
}


char *Base64Encrypt(Stringex *ciphertext,const void *b64_encode_this, int encode_this_many_bytes, Boolean with_new_line)
{

    BIO *b64_bio, *mem_bio;            // Declares two OpenSSL BIOs: a base64 filter and a memory BIO.
    BUF_MEM *mem_bio_mem_ptr;          // Pointer to a "memory BIO" structure holding our base64 data.
    b64_bio = BIO_new(BIO_f_base64()); // Initialize our base64 filter BIO.
    mem_bio = BIO_new(BIO_s_mem());    // Initialize our memory sink BIO.
    BIO_push(b64_bio, mem_bio);        // Link the BIOs by creating a filter-sink BIO chain.
    if (!with_new_line)
        BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);           // No newlines every 64 characters or less.
    BIO_write(b64_bio, b64_encode_this, encode_this_many_bytes);  // Records base64 encoded data.
    BIO_flush(b64_bio);                                           // Flush data.  Necessary for b64 encoding, because of pad characters.
    BIO_get_mem_ptr(mem_bio, &mem_bio_mem_ptr);                   // Store address of mem_bio's memory structure.
    BIO_set_close(mem_bio, BIO_NOCLOSE);                          // Permit access to mem_ptr after BIOs are destroyed.
    BIO_free_all(b64_bio);                                        // Destroys all BIOs in chain, starting with b64 (i.e. the 1st one).
    BUF_MEM_grow(mem_bio_mem_ptr, (*mem_bio_mem_ptr).length + 1); // Makes space for end null.
    (*mem_bio_mem_ptr).data[(*mem_bio_mem_ptr).length] = '\0';    // Adds null-terminator to tail.
	StringexCreate(ciphertext,(*mem_bio_mem_ptr).data,(*mem_bio_mem_ptr).length);
	BUF_MEM_free(mem_bio_mem_ptr);
    return ciphertext->valuestring;                               // Returns base-64 encoded data. (See: "buf_mem_st" struct).
}

char *Base64Decrypt(Stringex *plainText, const void *b64_decode_this, int decode_this_many_bytes, Boolean with_new_line)
{
	BIO *b64_bio, *mem_bio;
	plainText->valuestring = calloc((decode_this_many_bytes * 3) / 4 + 1, sizeof(char));
	b64_bio = BIO_new(BIO_f_base64());
	mem_bio = BIO_new(BIO_s_mem());
	BIO_write(mem_bio, b64_decode_this, decode_this_many_bytes);
	BIO_push(b64_bio, mem_bio);
	if (!with_new_line)
		BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL); // Don't require trailing newlines.
	plainText->valuelength = 0;						// Index where the next base64_decoded byte should be written.
	while (0 < BIO_read(b64_bio, plainText->valuestring + plainText->valuelength, 1))
	{								 // Read byte-by-byte.
		plainText->valuelength++; // Increment the index until read of BIO decoded data is complete.
	}
	// Once we're done reading decoded data, BIO_read returns -1 even though there's no error.
	BIO_free_all(b64_bio);			  // Destroys all BIOs in chain, starting with b64 (i.e. the 1st one).
	return plainText->valuestring; // Returns base-64 decoded data with trailing null terminator.
}