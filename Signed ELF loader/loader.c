#include <elf.h>
#include <errno.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char ** argv, char ** envp) {
  if ( argc != 3 ) {
    fprintf(stderr, "Usage: loader <secured binary> <public key>\n");
    return EXIT_FAILURE;
  }

  FILE * file = fopen(argv[1], "rb");
  if ( !file ) {
    fprintf(stderr, "Unable to open '%s': %s\n", argv[1], strerror(errno));
    return EXIT_FAILURE;
  }
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  fseek(file, 0L, SEEK_SET);

  char * buffer = malloc(size);
  if ( fread(buffer, size, 1, file) != 1 ) {
    fprintf(stderr, "fread: %s\n", strerror(errno));
    fclose(file);
    return EXIT_FAILURE;
  }
  fclose(file);

  Elf32_Ehdr * ehdr = (Elf32_Ehdr *) buffer;

  if(!ehdr
  || ehdr->e_ident[EI_MAG0] != ELFMAG0
  || ehdr->e_ident[EI_MAG1] != ELFMAG1
  || ehdr->e_ident[EI_MAG2] != ELFMAG2
  || ehdr->e_ident[EI_MAG3] != ELFMAG3) {
    fprintf(stderr, "'%s' is not an ELF file.\n", argv[1]);
    fclose(file);
    return EXIT_FAILURE;
  }

  Elf32_Shdr * shdr = (Elf32_Shdr *) (buffer + ehdr->e_shoff);
  Elf32_Shdr * sh_strtab = &shdr[ehdr->e_shstrndx];
  char * sh_strtab_p = buffer + sh_strtab->sh_offset;

  Elf32_Shdr * sha_shdr = NULL;
  for ( int i = 0; i < ehdr->e_shnum; ++i ) {
    if ( !strcmp(".sha", sh_strtab_p + shdr[i].sh_name) ) {
      sha_shdr = &shdr[i];
      break;
    }
  }
  if ( sha_shdr == NULL ) {
    fprintf(stderr, "Missing .sha section header.\n");
    return EXIT_FAILURE;
  }

  RSA *rsa = RSA_new();
  BIO *keybio = NULL;
  keybio = BIO_new(BIO_s_file());
  BIO_read_filename(keybio, argv[2]);
  rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
  EVP_PKEY* pubKey  = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(pubKey, rsa);

  unsigned char checksum[256];
  memcpy(checksum, (unsigned char *) buffer + sha_shdr->sh_offset, 256);
  printf("Loaded checksum;\n");
  for ( int i = 0; i < 256; ++i ) {
    printf("%x", checksum[i]);
  }
  printf("\n");

  char * objcopyargs[6] = {"objcopy", "--remove-section", ".sha", argv[1], "verifying", NULL};
  pid_t c_pid, pid;
  int status;

  c_pid = fork();
  if (c_pid == 0 ) {
    execvp("objcopy", objcopyargs);
    perror("execve failed");
  } else if ( c_pid > 0 ) {
    if( (pid = wait(&status)) < 0 )
      _exit(1);
  } else {
    perror("fork failed");
    _exit(1);
  }
  printf("Created verification copy.\n");

  FILE * verificationfile = fopen("verifying", "rb");
  if ( !verificationfile ) {
    fprintf(stderr, "Unable to open '%s': %s\n", "verifying", strerror(errno));
    return EXIT_FAILURE;
  }
  fseek(verificationfile, 0, SEEK_END);
  int verificationsize = ftell(verificationfile);
  fseek(verificationfile, 0L, SEEK_SET);

  unsigned char * verificationbuffer = malloc(verificationsize);
  if ( fread(verificationbuffer, verificationsize, 1, verificationfile) != 1 ) {
    fprintf(stderr, "fread: %s\n", strerror(errno));
    fclose(verificationfile);
    return EXIT_FAILURE;
  }
  fclose(verificationfile);

  EVP_MD_CTX * mdctx = NULL;
  if ( (mdctx = EVP_MD_CTX_create()) ) {
    printf("Created verification context.\n");
    if ( EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, pubKey) == 1 ) {
      printf("Initialized sha256 verification.\n");
      if ( EVP_DigestVerifyUpdate(mdctx, verificationbuffer, verificationsize) == 1 ) {
        printf("Created digest.\n");
        if ( EVP_DigestVerifyFinal(mdctx, checksum, EVP_PKEY_size(pubKey)) == 1 ) {
          printf("Verified!\n");
          char * callargs[2] = {"verifying", NULL};
          execvp("./verifying", callargs);
          return 0;
        }
      }
    }
  }
  printf("Verification failed.\n");
  return 0;
}
