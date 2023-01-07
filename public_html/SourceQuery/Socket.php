<?
    namespace xPaw\SourceQuery;
    use xPaw\SourceQuery\Exception\InvalidPacketException;
    use xPaw\SourceQuery\Exception\SocketException;
    class Socket extends BaseSocket
    {
        public function Close( ) : void
        {
            if( $this->Socket !== null )
            {
                fclose( $this->Socket );
                $this->Socket = null;
            }
        }
        public function Open( string $Address, int $Port, int $Engine ) : void
        {
            $this->Engine = $Engine;
            $this->Port = $Port;
            $this->Address = $Address;
            $Socket = @fsockopen( 'udp://' . $Address, $Port, $ErrNo, $ErrStr, 0.25 );
            if( $ErrNo || $Socket === false )
            {
                throw new SocketException( 'Socket creation failed: ' . $ErrStr, SocketException::COULD_NOT_CREATE_SOCKET );
            }
            $this->Socket = $Socket;
            stream_set_timeout( $this->Socket, 0, 250000 );
            stream_set_blocking( $this->Socket, true );
        }
        public function Write( int $Header, string $String = '' ) : bool
        {
            $Command = pack( 'ccccca*', 0xFF, 0xFF, 0xFF, 0xFF, $Header, $String );
            $Length = strlen( $Command );
            return $Length === fwrite( $this->Socket, $Command, $Length );
        }
        public function Read( int $Length = 65535 ) : Buffer
        {
            $Buffer = new Buffer( );
            $Buffer->Set( fread( $this->Socket, $Length ) );
            $this->ReadInternal( $Buffer, $Length, [ $this, 'Sherlock' ] );
            return $Buffer;
        }
        public function Sherlock( Buffer $Buffer, int $Length ) : bool
        {
            $Data = fread( $this->Socket, $Length );
            if( strlen( $Data ) < 4 )
            {
                return false;
            }
            $Buffer->Set( $Data );
            return $Buffer->GetLong( ) === -2;
        }
    }