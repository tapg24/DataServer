#include "utils/wxzip.h"

#include <memory>

#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include <wx/datstrm.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>
#include <wx/mstream.h>

namespace Util
{
	namespace Zip
	{
		std::string GetEntry( const std::string& filename, const std::string& entry )
		{
			FRL_EXCEPT_GUARD();

			std::auto_ptr<wxFFileInputStream> in( new wxFFileInputStream( filename ) );
			if( !in->IsOk() ) FRL_THROW_CLASS( FileNotExist, std::string("File not exist ") + filename);
			wxZipInputStream zip(filename, entry);
			if( !zip ) FRL_THROW_CLASS( FileNotExist, std::string("File not exist ") + entry );
			wxTextInputStream txt(zip);
			std::string result;
			wxString line;
			do 
			{
				line = txt.ReadLine();
				result.append(line);
			} while ( !line.IsEmpty() );

			return result;
		}

		void UpdateEntry( const std::string& filename, const std::string& entry, const std::string& data )
		{
			FRL_EXCEPT_GUARD();

			std::auto_ptr<wxFFileInputStream> in( new wxFFileInputStream( filename ) );
			if( !in.get() ) FRL_THROW_CLASS( FileNotExist, std::string("File not exist ") + filename );
			wxTempFileOutputStream out( filename );

			wxZipInputStream inzip( *in );
			wxZipOutputStream outzip( out );

			std::auto_ptr<wxZipEntry> zipEntry;

			outzip.CopyArchiveMetaData( inzip );

			while ( zipEntry.reset( inzip.GetNextEntry() ), zipEntry.get() != NULL )
			{
				if ( zipEntry->GetName().Matches(entry.c_str()) )
				{
					outzip.PutNextEntry( zipEntry->GetName() );
					wxTextOutputStream outtext( outzip );
					outtext << data;
				}
				else if ( !outzip.CopyEntry( zipEntry.release(), inzip ) )
				{
					break;
				}
			}

			in.reset();

			bool success = inzip.Eof() && outzip.Close() && out.Commit();
			if ( !success )
			{
				std::string msg = std::string() + filename + ", " + entry;
				FRL_THROW_CLASS( InvalidUpdate, msg );
			}
			//wxASSERT(success);
			//return 0;
		}

		void GetListEntry( const std::string& filename, std::list<std::string>& list )
		{
			FRL_EXCEPT_GUARD();

			if (!IsValidArchive(filename)) FRL_THROW_CLASS( InvalidArchive, std::string("Archive not valid") + filename );
			wxFFileInputStream in( filename );
			if( !in ) FRL_THROW_CLASS( FileNotExist, std::string("File not exist ") + filename );
			wxZipInputStream zip(in);
			std::auto_ptr<wxZipEntry> zipEntry;
			while ( zipEntry.reset( zip.GetNextEntry() ), zipEntry.get() != NULL )
			{
				list.push_back(zipEntry->GetName().mb_str());
			}
		}

		bool IsValidArchive( const std::string& filename )
		{
			FRL_EXCEPT_GUARD();

			wxFFileInputStream in( filename );
			if( !in ) FRL_THROW_CLASS( FileNotExist, std::string("File not exist ") + filename );
			wxZipInputStream zip(in);
			return !!zip.GetTotalEntries();

			//wxStreamError er = zip.GetLastError();
			//bool good = zip.IsOk();
			//zip.GetNextEntry();
			//if ( zip.GetLastError() == wxSTREAM_NO_ERROR) return true;
			//else return false;
		}

	}
}