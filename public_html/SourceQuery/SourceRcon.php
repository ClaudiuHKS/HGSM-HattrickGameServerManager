<? namespace xPaw\SourceQuery;
    use xPaw\SourceQuery\Exception\AuthenticationException;
    use xPaw\SourceQuery\Exception\InvalidPacketException;
    use xPaw\SourceQuery\Exception\SocketException;
    class SourceRcon {
        private BaseSocket $Socket;
        private $RconSocket;
        private int $RconRequestId = 0;
        public function __construct( BaseSocket $Socket ) {
            $this->Socket = $Socket;
        }
        public function Close( ) : void {
            if( $this->RconSocket ) {
                fclose( $this->RconSocket );
                $this->RconSocket = null;
            }
            $this->RconRequestId = 0;
        }
        public function Open( ) : void {
            if( !$this->RconSocket ) {
                $RconSocket = @fsockopen( $this->Socket->Address, $this->Socket->Port, $ErrNo, $ErrStr, 0.25 );
                if( $ErrNo || !$RconSocket ) {
                    throw new SocketException( 'Unable to connect: ' . $ErrStr, SocketException::CONNECTION_FAILED );
                }
                $this->RconSocket = $RconSocket;
                stream_set_timeout( $this->RconSocket, 0, 250000 );
                stream_set_blocking( $this->RconSocket, true );
            }
        }
        public function Write( int $Header, string $String = '' ) : bool {
            $Command = pack( 'VV', ++$this->RconRequestId, $Header ) . $String . "\x00\x00";
            $Command = pack( 'V', strlen( $Command ) ) . $Command;
            $Length = strlen( $Command );
            return $Length === fwrite( $this->RconSocket, $Command, $Length );
        }
        public function Read( ) : Buffer {
            $Buffer = new Buffer( );
            $Buffer->Set( fread( $this->RconSocket, 4 ) );
            if( $Buffer->Remaining( ) < 4 ) {
                throw new InvalidPacketException( 'Failed to read any data from the socket.', InvalidPacketException::BUFFER_EMPTY );
            }
            $PacketSize = $Buffer->GetLong( );
            $Buffer->Set( fread( $this->RconSocket, $PacketSize ) );
            $Data = $Buffer->Get( );
            $Remaining = $PacketSize - strlen( $Data );
            while( $Remaining > 0 ) {
                $Data2 = fread( $this->RconSocket, $Remaining );
                $PacketSize = strlen( $Data2 );
                if( $PacketSize === 0 ) {
                    throw new InvalidPacketException( 'Read ' . strlen( $Data ) . ' bytes from the socket and ' . $Remaining . ' bytes remaining.', InvalidPacketException::BUFFER_EMPTY );
                }
                $Data .= $Data2;
                $Remaining -= $PacketSize;
            }
            $Buffer->Set( $Data );
            return $Buffer;
        }
        public function Command( string $Command ) : string {
            $this->Write( SourceQuery::SERVERDATA_EXECCOMMAND, $Command );
            $Buffer = $this->Read( );
            $Buffer->GetLong( );
            $Type = $Buffer->GetLong( );
            if( $Type === SourceQuery::SERVERDATA_AUTH_RESPONSE ) {
                throw new AuthenticationException( 'Bad rcon_password.', AuthenticationException::BAD_PASSWORD );
            }
            else if( $Type !== SourceQuery::SERVERDATA_RESPONSE_VALUE ) {
                throw new InvalidPacketException( 'Invalid rcon response.', InvalidPacketException::PACKET_HEADER_MISMATCH );
            } $__tmpBuffer__ = $Buffer->Get(); {
                $Data = @Mb_Substr($__tmpBuffer__, 0, @Mb_Strlen($__tmpBuffer__) - 2);
            } $this->Write( SourceQuery::SERVERDATA_REQUESTVALUE );
            do {
                $Buffer = $this->Read( );
                $Buffer->GetLong( );
                if( $Buffer->GetLong( ) !== SourceQuery::SERVERDATA_RESPONSE_VALUE ) {
                    break;
                }
                $Data2 = $Buffer->Get( );
                if( $Data2 === "\x00\x01\x00\x00\x00\x00" ) {
                    break;
                }
                $Data .= @Mb_Substr($Data2, 0, @Mb_Strlen($Data2) - 2);
            } while( true );
            return rtrim( $Data, "\0" );
        }
        public function Authorize( string $Password ) : void {
            $this->Write( SourceQuery::SERVERDATA_AUTH, $Password );
            $Buffer = $this->Read( );
            $RequestID = $Buffer->GetLong( );
            $Type = $Buffer->GetLong( );
            if( $Type === SourceQuery::SERVERDATA_RESPONSE_VALUE ) {
                $Buffer = $this->Read( );
                $RequestID = $Buffer->GetLong( );
                $Type = $Buffer->GetLong( );
            }
            if( $RequestID === -1 || $Type !== SourceQuery::SERVERDATA_AUTH_RESPONSE ) {
                throw new AuthenticationException( 'Rcon authorization failed.', AuthenticationException::BAD_PASSWORD );
            }
        }
    }