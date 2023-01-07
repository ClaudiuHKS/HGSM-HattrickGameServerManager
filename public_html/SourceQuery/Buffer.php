<?
    namespace xPaw\SourceQuery;
    use xPaw\SourceQuery\Exception\InvalidPacketException;
    class Buffer
    {
        private string $Buffer = '';
        private int $Length = 0;
        private int $Position = 0;
        public function Set( string $Buffer ) : void
        {
            $this->Buffer = $Buffer;
            $this->Length = strlen( $Buffer );
            $this->Position = 0;
        }
        public function Remaining( ) : int
        {
            return $this->Length - $this->Position;
        }
        public function Get( int $Length = -1 ) : string
        {
            if( $Length === 0 )
            {
                return '';
            }
            $Remaining = $this->Remaining( );
            if( $Length === -1 )
            {
                $Length = $Remaining;
            }
            else if( $Length > $Remaining )
            {
                return '';
            }
            $Data = substr( $this->Buffer, $this->Position, $Length );
            $this->Position += $Length;
            return $Data;
        }
        public function GetByte( ) : int
        {
            return ord( $this->Get( 1 ) );
        }
        public function GetShort( ) : int
        {
            if( $this->Remaining( ) < 2 )
            {
                throw new InvalidPacketException( 'Not enough data to unpack a short.', InvalidPacketException::BUFFER_EMPTY );
            }
            $Data = unpack( 'v', $this->Get( 2 ) );
            return (int)$Data[ 1 ];
        }
        public function GetLong( ) : int
        {
            if( $this->Remaining( ) < 4 )
            {
                throw new InvalidPacketException( 'Not enough data to unpack a long.', InvalidPacketException::BUFFER_EMPTY );
            }
            $Data = unpack( 'l', $this->Get( 4 ) );
            return (int)$Data[ 1 ];
        }
        public function GetFloat( ) : float
        {
            if( $this->Remaining( ) < 4 )
            {
                throw new InvalidPacketException( 'Not enough data to unpack a float.', InvalidPacketException::BUFFER_EMPTY );
            }
            $Data = unpack( 'f', $this->Get( 4 ) );
            return (float)$Data[ 1 ];
        }
        public function GetUnsignedLong( ) : int
        {
            if( $this->Remaining( ) < 4 )
            {
                throw new InvalidPacketException( 'Not enough data to unpack an usigned long.', InvalidPacketException::BUFFER_EMPTY );
            }
            $Data = unpack( 'V', $this->Get( 4 ) );
            return (int)$Data[ 1 ];
        }
        public function GetString( ) : string
        {
            $ZeroBytePosition = strpos( $this->Buffer, "\0", $this->Position );
            if( $ZeroBytePosition === false )
            {
                return '';
            }
            $String = $this->Get( $ZeroBytePosition - $this->Position );
            $this->Position++;
            return $String;
        }
    }