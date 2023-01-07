<?
    namespace xPaw\SourceQuery;
    use xPaw\SourceQuery\Exception\AuthenticationException;
    use xPaw\SourceQuery\Exception\InvalidPacketException;
    class GoldSourceRcon
    {
        private $Socket;
        private string $RconPassword = '';
        private string $RconChallenge = '';
        public function __construct( BaseSocket $Socket )
        {
            $this->Socket = $Socket;
        }
        public function Close( ) : void
        {
            $this->RconChallenge = '';
            $this->RconPassword = '';
        }
        public function Open( ) : void
        {
        }
        public function Write( int $Header, string $String = '' ) : bool
        {
            $Command = pack( 'cccca*', 0xFF, 0xFF, 0xFF, 0xFF, $String );
            $Length = strlen( $Command );
            return $Length === fwrite( $this->Socket->Socket, $Command, $Length );
        }
        public function Read( int $Length = 65535 ) : Buffer
        {
            $Buffer = $this->Socket->Read();
            $StringBuffer = '';
            $ReadMore = false;
            do {
                $ReadMore = $Buffer->Remaining() > 0;
                if ($ReadMore) {
                    if ($Buffer->GetByte() !== SourceQuery::S2A_RCON) {
                        throw new InvalidPacketException('Invalid rcon response.', InvalidPacketException::PACKET_HEADER_MISMATCH);
                    } $Packet = $Buffer->Get();
                    $StringBuffer .= @Mb_Substr($Packet, 0, @Mb_Strlen($Packet) - 2);
                    $ReadMore = strlen($Packet) > 1316;
                    if ($ReadMore) {
                        $Buffer = $this->Socket->Read();
                    }
                }
            } while ($ReadMore);
            $Trimmed = trim($StringBuffer);
            if ($Trimmed === 'Bad rcon_password.') {
                throw new AuthenticationException($Trimmed, AuthenticationException::BAD_PASSWORD);
            } else if ($Trimmed === 'You have been banned from this server.') {
                throw new AuthenticationException($Trimmed, AuthenticationException::BANNED);
            } $Buffer->Set($Trimmed);
            return $Buffer;
        }
        public function Command( string $Command ) : string
        {
            if( !$this->RconChallenge )
            {
                throw new AuthenticationException( 'Tried to execute a command before successful authorization.', AuthenticationException::BAD_PASSWORD );
            }
            $this->Write( 0, 'rcon ' . $this->RconChallenge . ' "' . $this->RconPassword . '" ' . $Command . "\0" );
            $Buffer = $this->Read( );
            return $Buffer->Get( );
        }
        public function Authorize( string $Password ) : void
        {
            $this->RconPassword = $Password;
            $this->Write( 0, 'challenge rcon' );
            $Buffer = $this->Socket->Read( );
            if( $Buffer->Get( 14 ) !== 'challenge rcon' )
            {
                throw new AuthenticationException( 'Failed to get rcon challenge.', AuthenticationException::BAD_PASSWORD );
            }
            $this->RconChallenge = trim( $Buffer->Get( ) );
        }
    }