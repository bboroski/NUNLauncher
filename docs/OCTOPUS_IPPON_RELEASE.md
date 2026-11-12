# Octopus Ippon Release — NUNLauncher

Project profile: host-side kernel image admission and boot-information contract validator.

Release gates: CMake host build, CTest, documented test suite and repository checker; source archive; SPDX SBOM; SHA-256 and provenance attestation. A host validator does not execute the image or prove firmware/hardware handoff. QEMU and physical target qualification remain separate evidence.

Version tags publish only after all host-side gates pass.