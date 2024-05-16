# nosmart

## what is s.m.a.r.t technology?

the technological definition:
> SMART is a system that monitors the internal information of your drive. Its clever name is actually an acronym for Self-Monitoring, Analysis, and Reporting Technology. SMART, also written as S.M.A.R.T., is a technology found inside HDDs and SSDs. It is independent of your operating system, BIOS, or other software.

the "actual" definition:

> used by different 3rd-party software to determine your hardware identifier serials, to then fingerprint your PC entirely. mainly used by anti-cheat technologies.

## how does this work?

this project is an example to manipulate physical memory inside the context of another module, to interfere with their functions without messing with any 3rd-party-software (AC's, AV's ,etc..)'s stackwalking routines

## what does the driver do?

simply exploits an important check determined to inform the driver itself off s.m.a.r.t presence.
(also, yes, while writing this project I have figured the mem block scan for their device control works also perfectly to scan the disassembled addresses. maybe +11 or +41 are holders for some other structure which those addresses are based off? or a pure stack/aslr/offsetting coincidence? not sure.)
```cpp
   if ( a5 )
    {
      if ( (unsigned int)(a3 - 1) <= 2 || a3 == 5 )
      {
        v10 = DiskEnableDisableFailurePrediction(*(_QWORD *)(a1 + 64), 1);
        v6 = v10;
        if ( WPP_GLOBAL_Control != &WPP_GLOBAL_Control
          && (*((_DWORD *)WPP_GLOBAL_Control + 11) & 0x40) != 0
          && *((_BYTE *)WPP_GLOBAL_Control + 41) >= 4u )
        {
          WPP_SF_qqL(*((_QWORD *)WPP_GLOBAL_Control + 3), 33LL, v11, a1, v8, v10);
        }
      }
      else if ( WPP_GLOBAL_Control != &WPP_GLOBAL_Control
             && (*((_DWORD *)WPP_GLOBAL_Control + 11) & 0x40) != 0
             && *((_BYTE *)WPP_GLOBAL_Control + 41) >= 4u )
      {
        WPP_SF_qqds(*((_QWORD *)WPP_GLOBAL_Control + 3), 34, a3, a1, a2, a3, (__int64)"Enabled");
      }
    }
```
