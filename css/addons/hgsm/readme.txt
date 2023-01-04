HGSM (Hattrick Game Server Manager) For HL2 Games

    Files Needed Here (/%GameFolder%/addons/hgsm/*)

        apply_console_variables.cfg         Can Alter Hidden Console Variables For Example The "mp_flashlight" Is Hidden In CS:GO               (JSON Type)
        offs.cfg                            Offsets For Example Windows And Linux Differences                                                   (JSON Type)
        reveal_console_variables.cfg        Reveal (Show) Some ConVars, Hidden Or Not, Display Details About Them Into The Server Console       (JSON Type)
        server_tick_rate_and_priority.cfg   The Main Configuration File                                                                         (JSON Type)

        sys.cfg                             Commands To Be Sent To The System Via Cmd/ Terminal                                                 (INI Type)

        readme.txt                          Details                                                                                             (TXT Type)

    Optional Files Here (/%GameFolder%/addons/hgsm/*)

        delete_bot_profile_files            If It Ever Exists Here The Files Like "/%GameFolder%/botchatter.db" (*.db) Will Always Be Erased - Useful If Your Game Server Is Not Using Playing Bots
        delete_motd_file                    If It Ever Exists Here The "/%GameFolder%/motd.txt" And The "/%GameFolder%/motd_text.txt" Files Will Always Be Removed

        autoexec.cfg                        If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        config.cfg                          If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        game.cfg                            If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        gamemode_competitive.cfg            If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        listenserver.cfg                    If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        skill.cfg                           If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        skill1.cfg                          If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files

        valve.rc                            If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files

        bspconvar_whitelist.txt             If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        gamemodes_server.txt                If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        gamerulescvars.txt                  If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        mapcycle.txt                        If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        mapcycle_default.txt                If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        maplist.txt                         If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        missioncycle.txt                    If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        motd.txt                            If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        motd_default.txt                    If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files
        motd_text.txt                       If It Ever Exists Here It Will Be Copied Where It Belongs - Sometimes The Steam Cmd Update Overrides Some Of Your Files

    More Information

        You Can Have "27015.cfg" - A Configuration File For Each Port You Are Using Which Will Be Executed When The Map Starts

        mem.txt - On Linux, It Shows Memory (RAM) Information From The System (Size, Frequency, ..)
        cpu.txt - On Linux, It Shows Processor (CPU) Information From The System (Frequency, Cores, Threads, Size, ..)

        If The "motd.txt" And The "delete_motd_file" Files Exist Here Then The "delete_motd_file" File Will Be Considered
        If The "motd_default.txt" And The "delete_motd_file" Files Exist Here Then The "delete_motd_file" File Will Be Considered
        If The "motd_text.txt" And The "delete_motd_file" Files Exist Here Then The "delete_motd_file" File Will Be Considered
        If The "motd.txt", The "motd_default.txt", The "motd_text.txt" And The "delete_motd_file" Files Exist Here Then The "delete_motd_file" File Will Be Considered
