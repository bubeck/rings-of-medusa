          move.b    $ffff8001,d1        ;Memoryconfiguration
          cmpi.l    #'MEM!',$144        ;spezielle Memoryconfiguration?
          bne.s     no_512
          move.w    $148,d1   ;holen
no_512:   andi.w    #%1111,d1
          lsl.w     #1,d1
          move.w    speicher(pc,d1.w),d1
          mulu.w    #1024,d1  ;=Speicher in Bytes
          sub.l     d0,d1     ;=Freie Bytes
          move.l    d1,mem_len

          bra       to_main

;Verschiedene Speicherausbaustufen:
speicher: .DC.w 256
          .DC.w 640
          .DC.w 2176
          .DC.w 512
          .DC.w 640
          .DC.w 1024
          .DC.w 2560
          .DC.w 512
          .DC.w 2176
          .DC.w 2560
          .DC.w 4096
          .DC.w 512
          .DC.w 512,512,512,512

