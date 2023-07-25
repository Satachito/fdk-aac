#include <aacdecoder_lib.h>

#include "JP.h"
using namespace JP;

vector< UI1 >
Main( const vector< UI1 >& _ ) {

	vector< UI1 > $;

	auto decoder = aacDecoder_Open( TT_MP4_ADTS, 1 );
	auto len = UI4( _.size() );
	auto remain = len;
	while ( remain ) {
		auto p = (UI1*)_.data();
		A( aacDecoder_Fill( decoder, &p, &len, &remain ) == AAC_DEC_OK );
//	cerr << remain << endl;
		while ( true ) {
			INT_PCM outbuf[ 20480 ];
			auto er = aacDecoder_DecodeFrame( decoder, outbuf, 20480, 0 );
			if ( er == AAC_DEC_NOT_ENOUGH_BITS ) break;
			A( er == AAC_DEC_OK );
			auto SI = aacDecoder_GetStreamInfo( decoder );
//	cerr
//	<< "sampleRate: "		<< SI->sampleRate
//	<< ", bitRate: "		<< SI->bitRate
//	<< ", frameSize: "		<< SI->frameSize
//	<< ", numChannels: "	<< SI->numChannels
//	<< endl
//	;
			$.insert( $.end(), (UI1*)outbuf, (UI1*)outbuf + 2 * SI->frameSize * SI->numChannels );
		}
	}
	aacDecoder_Close(decoder);
	return $;
}

int
main(int argc, char const *argv[]) {
	if(argc<3){
		fprintf(stderr,"Usage: aac-dec [in-aac-file] [out-pcm-file]");
		return 1;
	}
	SetFileContent( argv[ 2 ], Main( GetFileContent( argv[ 1 ] ) ) );
}
