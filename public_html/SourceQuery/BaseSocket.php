<? namespace xPaw\SourceQuery; use xPaw\SourceQuery\Exception\sckExcp;
    abstract class BaseSocket {
        public $Socket;
        public int $Engine;
        public bool $fromRcon = False;
        public string $Address;
        public int $Port;
        public function __destruct( ) { $this->fromRcon = False; $this->Close( ); }
        abstract public function Close( ) : void;
        abstract public function Open( string $Address, int $Port, int $Engine ) : void;
        abstract public function Write( int $Header, string $String = '' ) : bool;
        abstract public function Read( int $Length = 65535 ) : Buffer;
        protected function ReadInternal( Buffer $Buffer, int $Length, callable $SherlockFunction ) : Buffer {
            If( False === $this->fromRcon && $Buffer->Remaining( ) === 0 ) { throw new sckExcp( 'Has nothing to read.', sckExcp::CONNECTION_FAILED ); }
            $Header = $Buffer->GetLong( );
            if( $Header === -1 ) {
            } else if( $Header === -2 ) {
                $Packets = [];
                $IsCompressed = false;
                $ReadMore = false;
                $PacketChecksum = null;
                do { $RequestID = $Buffer->GetLong( );
                    switch( $this->Engine ) {
                        case SourceQuery::GOLDSOURCE: {
                            $PacketCountAndNumber = $Buffer->GetByte( );
                            $PacketCount = $PacketCountAndNumber & 0xF;
                            $PacketNumber = $PacketCountAndNumber >> 4;
                            break;
                        } case SourceQuery::SOURCE: {
                            $IsCompressed = ( $RequestID & 0x80000000 ) !== 0;
                            $PacketCount = $Buffer->GetByte( );
                            $PacketNumber = $Buffer->GetByte( ) + 1;
                            if( $IsCompressed ) {
                                $Buffer->GetLong( );
                                $PacketChecksum = $Buffer->GetUnsignedLong( );
                            } else { $Buffer->GetShort( ); }
                            break;
                        } default: { throw new sckExcp( 'Unknown engine.', sckExcp::INVALID_ENGINE ); }
                    } $Packets[ $PacketNumber ] = $Buffer->Get( );
                    $ReadMore = $PacketCount > sizeof( $Packets );
                } while( $ReadMore && $SherlockFunction( $Buffer, $Length ) );
                $Data = implode( $Packets );
                if( $IsCompressed ) { $Data = bzdecompress( $Data ); }
                $Buffer->Set( substr( $Data, 4 ) );
            } return $Buffer;
        }
    }