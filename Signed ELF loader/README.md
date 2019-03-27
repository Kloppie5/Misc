# Signed ELF loader

Loads ELF files and only executes them if their sha256 signature corresponds to a provided public key.

Potential improvements to be made;
- Currently executes a copy of the ELF file without the signature named verifying, which remains after running.
- Only works with with static addressing and sha256.
