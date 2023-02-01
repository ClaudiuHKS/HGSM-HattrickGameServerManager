<?
    Require __DIR__ . '/SourceQuery/bootstrap.php';

    Use xPaw\SourceQuery\SourceQuery;

    //
    /// CFG
    //

    $Idx = "RCon Studio";

    $Usr = @Hash("sha3-512", "Hattrick");
    $Pwd = @Hash("sha3-512", "...................");

    $Dns = 'https://hattrick.go.ro/';
    $Hdr = 'Location: https://hattrick.go.ro/Rcon.Php';

    $Ses = Array
    (
        'cookie_lifetime' =>    0,

        'cache_expire' =>       0,
        'cache_limiter' =>      'nocache',
    );

    $Srv = Array
    (
        "hattrick.go.ro",
        "hattrick.go.ro",
        "hattrick.go.ro",
        "hattrick.go.ro",

        "192.168.26.1",
        "192.168.26.1",
        "192.168.26.1",
        "192.168.26.1",
        "192.168.26.1",
    );

    $Prt = Array
    (
        27105,
        27115,
        27125,
        27135,

        27145,
        27155,
        27165,
        27175,
        27185,
    );

    $Eng = Array
    (
        SourceQuery::GOLDSOURCE,
        SourceQuery::GOLDSOURCE,
        SourceQuery::GOLDSOURCE,
        SourceQuery::GOLDSOURCE,

        SourceQuery::SOURCE,
        SourceQuery::SOURCE,
        SourceQuery::SOURCE,
        SourceQuery::SOURCE,
        SourceQuery::SOURCE,
    );

    $Pas = Array
    (
        "...................",
        "...................",
        "...................",
        "...................",

        "...................",
        "...................",
        "...................",
        "...................",
        "...................",
    );

    //
    /// CFG
    //

    If (Empty($_SERVER['HTTPS']))
    {
        Header($Hdr);
        {
            Exit;
        }
    }

    Else If (!Isset($_SESSION))
    {
        @Session_Start($Ses);
    }

    If ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        $_SESSION['postdata'] = $_POST;
        {
            Unset($_POST);
            {
                Header($Hdr);
                {
                    Exit;
                }
            }
        }
    }

    Else If ($_SESSION['postdata'])
    {
        $_POST = $_SESSION['postdata'];
        {
            Unset($_SESSION['postdata']);
        }
    }

    If (Isset($_POST['usr']))
    {
        @Setcookie('rcon_usr', @Hash("sha3-512", $_POST['usr']), @Time() + (86400 * 7 * 4));
        {
            If (Isset($_POST['pwd']))
            {
                @Setcookie('rcon_pwd', @Hash("sha3-512", $_POST['pwd']), @Time() + (86400 * 7 * 4));
                {
                    Header($Hdr);
                    {
                        Exit;
                    }
                }
            }

            Else
            {
                Header($Hdr);
                {
                    Exit;
                }
            }
        }
    }

    Else If (Isset($_POST['pwd']))
    {
        @Setcookie('rcon_pwd', @Hash("sha3-512", $_POST['pwd']), @Time() + (86400 * 7 * 4));
        {
            If (Isset($_POST['usr']))
            {
                @Setcookie('rcon_usr', @Hash("sha3-512", $_POST['usr']), @Time() + (86400 * 7 * 4));
                {
                    Header($Hdr);
                    {
                        Exit;
                    }
                }
            }

            Else
            {
                Header($Hdr);
                {
                    Exit;
                }
            }
        }
    }

    Echo "  <title>
                " . $Idx . "
            </title>

            <link rel=\"shortcut icon\" href=\"" . $Dns . "terminal.ico\" />
            <link rel=\"icon\" href=\"" . $Dns . "terminal.ico\" />

            <style>

                html *
                {
                    background-color:   white;
                    font-size:          12;
                    color:              black;
                    font-family:        Consolas;
                }

                td, th
                {
                    padding-left:       10;
                    padding-right:      10;
                    padding-top:        2;
                    padding-bottom:     2;
                }

                form
                {
                    margin:             0 auto;
                }

                img
                {
                    width:              24;
                    height:             24;
                }

                th
                {
                    text-align:         left;
                }

                input[type=text]
                {
                    width:              200;
                }

                input[type=password]
                {
                    width:              200;
                }

                textarea
                {
                    width:              750;
                    height:             250;
                }

            </style>";

    If (!Isset($_COOKIE['rcon_usr']) || Trim($_COOKIE['rcon_usr']) != $Usr)
    {
        Echo "  <form method=post>
                    USR <input type=text required name=usr />
                    <BR />
                    <BR />
                    PWD <input type=password required name=pwd />
                    <BR />
                    <BR />
                    <input type=submit value=ENTER />
                </form>";
        {
            Exit;
        }
    }

    If (!Isset($_COOKIE['rcon_pwd']) || Trim($_COOKIE['rcon_pwd']) != $Pwd)
    {
        Echo "  <form method=post>
                    USR <input type=text required name=usr />
                    <BR />
                    <BR />
                    PWD <input type=password required name=pwd />
                    <BR />
                    <BR />
                    <input type=submit value=ENTER />
                </form>";
        {
            Exit;
        }
    }

    $Qry = New SourceQuery();

    $Exc = Null;
    $Inf = [];

    Echo "  <table>

                <tr>

                    <th>
                        #
                    </th>
                    <th>
                        Sys
                    </th>
                    <th>
                        Game
                    </th>
                    <th>
                        Name
                    </th>
                    <th>
                        Map
                    </th>
                    <th>
                        Num
                    </th>
                    <th>
                        Max
                    </th>
                    <th>
                        Cmd
                    </th>

                </tr>";

    For ($Itr = 0; $Itr < Sizeof($Srv); $Itr++)
    {
        $Exc = Null;
        {
            $Inf = [];
        }

        Try
        {
            $Qry->Connect($Srv[$Itr], $Prt[$Itr], $Eng[$Itr]);
            {
                $Inf = $Qry->GetInfo();
            }
        }

        Catch (Exception $_)
        {
            $Exc = $_;
        }

        Finally
        {
            $Qry->Disconnect();
        }

        If ($Exc == Null)
        {
            If (!Empty($Inf))
            {
                $Plr = @Max((Int)$Inf['Players'], (Int)$Inf['Bots']);

                $Map = @Mb_Strtolower(Trim($Inf['Map']));
                {
                    If (@Ctype_Alnum($Map[0]))
                    {
                        If ($Map[1] == '_')
                        {
                            $Map = @Mb_SubStr($Map, 2);
                        }

                        Else If ($Map[2] == '_')
                        {
                            $Map = @Mb_SubStr($Map, 3);
                        }

                        Else If ($Map[3] == '_')
                        {
                            $Map = @Mb_SubStr($Map, 4);
                        }

                        Else If ($Map[4] == '_')
                        {
                            $Map = @Mb_SubStr($Map, 5);
                        }

                        Else If ($Map[5] == '_')
                        {
                            $Map = @Mb_SubStr($Map, 6);
                        }

                        $Map[0] = @Mb_Strtoupper($Map[0]);
                    }
                }

                Echo @Sprintf(" <tr>

                                    <td>
                                        %d
                                    </td>
                                    <td>
                                        <img src=\"%s%s.png\" />
                                    </td>
                                    <td>
                                        <img src=\"%s%s/%s.%s\" />
                                    </td>
                                    <td>
                                        %s
                                    </td>
                                    <td>
                                        %s
                                    </td>
                                    <td>
                                        %d
                                    </td>
                                    <td>
                                        %d
                                    </td>
                                    <td>
                                        <form method=post>

                                            <input type=hidden name=srv value=%d />
                                            <input type=text name=cmd required value=\"%s\" />
                                            <input type=submit value=SEND />

                                        </form>
                                    </td>

                                </tr>",
                                $Itr + 1,
                                $Dns,
                                (@Mb_Strtolower($Inf['Os']) == 'w') ? "winxp" : "ubuntu",
                                $Dns,
                                ((Int)$Inf['Protocol']) == 48 ? "games_goldsrc" : "games_src",
                                @Mb_Strtolower(Trim($Inf['ModDir'])),
                                ((Int)$Inf['Protocol']) == 48 ? "png" : "png",
                                @Htmlspecialchars(Trim($Inf['HostName'])),
                                $Map,
                                (Int)$Plr,
                                ((Int)$Inf['MaxPlayers']),
                                $Itr,
                                (Isset($_POST['cmd']) && Isset($_POST['srv']) && ((Int)$_POST['srv']) == $Itr) ? @Htmlentities(Trim($_POST['cmd'])) : "");
            }

            Else
            {
                Echo @Sprintf(" <tr>

                                    <td>
                                        %d
                                    </td>
                                    <td>
                                        ?
                                    </td>
                                    <td>
                                        ?
                                    </td>
                                    <td>
                                        ?
                                    </td>
                                    <td>
                                        ?
                                    </td>
                                    <td>
                                        ?
                                    </td>
                                    <td>
                                        ?
                                    </td>
                                    <td>
                                        ?
                                    </td>

                                </tr>",
                                $Itr + 1);
            }
        }

        Else
        {
            Echo @Sprintf(" <tr>

                                <td>
                                    %d
                                </td>
                                <td>
                                    ?
                                </td>
                                <td>
                                    ?
                                </td>
                                <td>
                                    ?
                                </td>
                                <td>
                                    ?
                                </td>
                                <td>
                                    ?
                                </td>
                                <td>
                                    ?
                                </td>
                                <td>
                                    ?
                                </td>

                            </tr>",
                            $Itr + 1);
        }
    }

    Echo "  </table>

            <table>
                <tr>
                    <td>";

    If (Isset($_POST['srv']) && Isset($_POST['cmd']))
    {
        $Res = "Failed.";
        {
            $Exc = Null;
        }

        Try
        {
            $Qry->Connect($Srv[@Abs(((Int)$_POST['srv']))], $Prt[@Abs(((Int)$_POST['srv']))], $Eng[@Abs(((Int)$_POST['srv']))]);
            {
                $Qry->SetRconPassword($Pas[@Abs(((Int)$_POST['srv']))]);
                {
                    $Res = Trim($Qry->Rcon(Trim($_POST['cmd'])));
                }
            }
        }

        Catch (Exception $_)
        {
            $Exc = $_;
        }

        Finally
        {
            $Qry->Disconnect();
        }

        If ($Exc == Null)
        {
            If (@Mb_Strlen($Res) > 0)
            {
                Echo "<textarea spellcheck=false>";
                {
                    Echo $Res;
                }
                Echo "</textarea>";
            }

            Else
            {
                Echo "<textarea spellcheck=false>";
                {
                    Echo Trim($_POST['cmd']);
                }
                Echo "</textarea>";
            }
        }

        Else
        {
            Echo "<textarea spellcheck=false>";
            {
                If (@Mb_Strlen(Trim($Exc->getMessage())) > 0)
                {
                    Echo Trim($Exc->getMessage());
                }

                Else
                {
                    Echo "Failed.";
                }
            }
            Echo "</textarea>";
        }
    }

    Echo "          </td>
                </tr>

                <tr>
                    <td>
                        <button onclick=\"window.location.reload();\">REFRESH PAGE</button>
                    </td>
                </tr>

            </table>";
?>
