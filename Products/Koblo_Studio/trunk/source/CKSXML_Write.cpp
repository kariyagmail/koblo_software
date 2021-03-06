
#include "KSOS.h"


CKSXML_Write::CKSXML_Write(CKSPlugIn * pKSPlugIn)
{
	
	mpKSPlugIn = pKSPlugIn;
	
}

CKSXML_Write::~CKSXML_Write()
{
	
}
void CKSXML_Write::Write_XML(std::string str)
{
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	
	TiXmlDocument *pDoc		=	new TiXmlDocument("koblo_doc");
	pDoc->LinkEndChild( decl );
	
	
	Add_Comment(pDoc, "Koblo Studio music project XML format");
	Add_Comment(pDoc, "most ids are originally returned from the website, and are globally unique (between different projects)");
	Add_Comment(pDoc, "ids are always specified as attributes");
	Add_Comment(pDoc, "all times and track positions are specified in sample points");
	
	Write_Project(pDoc);

	pDoc->SaveFile( str.c_str() );
	
	


	
}

void CKSXML_Write::Write_Project(TiXmlDocument* pDoc)
{
	
	TiXmlElement* pProject = new TiXmlElement( "project" );
//	TiXmlText * text = new TiXmlText( "World" );
//	pProject->LinkEndChild( text );
	
	pProject->SetAttribute("id",123);
	pProject->SetAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
	pProject->SetAttribute("xsi:noNamespaceSchemaLocation","http://koblo.com/schemas/koblo_project.xsd");
	
	
	pDoc->LinkEndChild( pProject );
	
	Write_Branch( pProject);
	Write_Settings( pProject);
	Write_Editing( pProject);
	Write_Samples(pProject);
	Write_Tracks(pProject);
	Write_Busses(pProject);
	
}

void CKSXML_Write::Write_Branch(TiXmlElement* pParent)
{
	
	Add_Comment(pParent, "information about the branch");
	
	TiXmlElement* pBranch = new TiXmlElement( "branch" );
	// ID
	pBranch->SetAttribute("id",123);
	pParent->LinkEndChild( pBranch );
	
	
	// name
	TiXmlElement* pName = new TiXmlElement( "name" );
	TiXmlText* pNameTxt = new TiXmlText("Resonance Chamber Edition");
	pName->LinkEndChild( pNameTxt );
	pBranch->LinkEndChild( pName );
	
	// description
	TiXmlElement* pDescription = new TiXmlElement( "description" );
	TiXmlText* pDescriptionTxt = new TiXmlText("A branch with extra high levels of resonance");
	pDescription->LinkEndChild( pDescriptionTxt );
	pBranch->LinkEndChild( pDescription );
	
	// revision
	TiXmlElement* pRevision = new TiXmlElement( "revision" );
	TiXmlText* pRevisionTxt = new TiXmlText("67");
	pRevision->LinkEndChild( pRevisionTxt );
	pBranch->LinkEndChild( pRevision );
	
	
}

//----------------------------------------------------------------
// settings
//----------------------------------------------------------------

void CKSXML_Write::Write_Settings(TiXmlElement* pParent)
{
	Add_Comment(pParent, "overall project settings");
	
	TiXmlElement* pSettings = new TiXmlElement( "settings" );
	pParent->LinkEndChild( pSettings );
	
	// name
	TiXmlElement* pName = new TiXmlElement( "name" );
	TiXmlText* pNameTxt = new TiXmlText("Crystal Cave");
	pName->LinkEndChild( pNameTxt );
	pSettings->LinkEndChild( pName );
	
	// description
	TiXmlElement* pDescription = new TiXmlElement( "description" );
	TiXmlText* pDescriptionTxt = new TiXmlText("This project uses samples from underground crystal caves in Mexico.");
	pDescription->LinkEndChild( pDescriptionTxt );
	pSettings->LinkEndChild( pDescription );
	

	char pszBuff [64];
	
	// Samplerate
	sprintf(pszBuff, "%d", mpKSPlugIn->GetSampleRate());
	TiXmlElement* pSampleRate = new TiXmlElement( "samplerate" );
	TiXmlText* pSampleRateTxt = new TiXmlText(pszBuff);
	pSampleRate->LinkEndChild( pSampleRateTxt );
	pSettings->LinkEndChild( pSampleRate );
	
	//Tempo
	tfloat32 fTempo = (tfloat32)mpKSPlugIn->GetGlobalParm(giParamID_KS_Tempo, giSectionGlobal) / 10.0f;
	sprintf(pszBuff, "%f", fTempo);
	TiXmlElement* pTempo = new TiXmlElement( "tempo" );
	TiXmlText* pTempoTxt = new TiXmlText(pszBuff);
	pTempo->LinkEndChild( pTempoTxt );
	pSettings->LinkEndChild( pTempo );
	
	TiXmlElement* pSignatur = new TiXmlElement( "signature" );;
	pSettings->LinkEndChild( pSignatur );
	Write_Signature(pSignatur);
	
	TiXmlElement* pLicense = new TiXmlElement( "license" );;
	pSettings->LinkEndChild( pLicense );
	Write_License(pLicense);
	
	
}

void CKSXML_Write::Write_Signature(TiXmlElement* pParent)
{
	tuint32 uiSignatur = mpKSPlugIn->GetGlobalParm(giParamID_KS_Time_Signature, giSectionGlobal);
	tuint uiNumerator;
	tuint uiDenominator;
	
	switch(uiSignatur){
			
		case giSignature2_4:{
			uiNumerator			= 2;
			uiDenominator		= 4;
			break;
		}
		case giSignature3_4:{
			uiNumerator			= 3;
			uiDenominator		= 4;
			break;
		}
		default:
		case giSignature4_4:{
			uiNumerator			= 4;
			uiDenominator		= 4;
			break;
		}
		case giSignature6_8:{
			uiNumerator			= 6;
			uiDenominator		= 8;
			break;
		}
		case giSignature9_8:{
			uiNumerator			= 9;
			uiDenominator		= 8;
			break;
		}
		case giSignature12_8:{
			uiNumerator			= 12;
			uiDenominator		= 8;
			break;
		}
	}
			
		
	char pszBuff [64];
	// numerator
	sprintf(pszBuff, "%d", uiNumerator);
	TiXmlElement* pNumerator = new TiXmlElement( "numerator" );
	TiXmlText* pNumeratorTxt = new TiXmlText(pszBuff);
	pNumerator->LinkEndChild( pNumeratorTxt );
	pParent->LinkEndChild( pNumerator );
	
	//Tempo
	sprintf(pszBuff, "%d", uiDenominator);
	TiXmlElement* pDenominator = new TiXmlElement( "tempo" );
	TiXmlText* pDenominatorTxt = new TiXmlText(pszBuff);
	pDenominator->LinkEndChild( pDenominatorTxt );
	pParent->LinkEndChild( pDenominator );
	
}

void CKSXML_Write::Write_License(TiXmlElement* pParent)
{
	char pszBuff [64];
	tuint uiTest	=	 mpKSPlugIn->GetGlobalParm(giParamID_CC_License_Type, giSectionGUI);
	
	switch(uiTest){
			
		case giCC_By:{
			sprintf(pszBuff, "by");
			break;
		}
		case giCC_By_Sa:{
			sprintf(pszBuff, "by-sa");
			break;
		}
		case giCC_By_Nd:{
			sprintf(pszBuff, "by-nd");
			break;
		}
			
		case giCC_By_Nc:{
			sprintf(pszBuff, "by-nc");
			break;
		}
		case giCC_By_Nc_Sa:{
			sprintf(pszBuff, "by-nc-sa");
			break;
		}
		case giCC_By_Nc_Nd:{
			sprintf(pszBuff, "by-nc-nd");
			break;
		}
	}
	TiXmlText* pLicenseTxt = new TiXmlText(pszBuff);
	pParent->LinkEndChild( pLicenseTxt );

}

//----------------------------------------------------------------
// editing
//----------------------------------------------------------------

void CKSXML_Write::Write_Editing(TiXmlElement* pParent)
{
	Add_Comment(pParent, "editing settings. these are things that would typically not be moved/merged between branches");
	char pszBuff [64];
	
	TiXmlElement* pEditing = new TiXmlElement( "editing" );
	pParent->LinkEndChild( pEditing );
	
	// tool
	TiXmlElement* pTool = new TiXmlElement( "tool" );;
	pEditing->LinkEndChild( pTool );
	Write_Tool(pTool);
	
	// zoom
	tint iVal = mpKSPlugIn->GetGlobalParm(giParamID_Zoom, giSectionGUI);
	sprintf(pszBuff, "%d", iVal);
	TiXmlElement* pZoom = new TiXmlElement( "zoom" );
	TiXmlText* pZoomTxt = new TiXmlText(pszBuff);
	pZoom->LinkEndChild( pZoomTxt );
	pEditing->LinkEndChild( pZoom );
	
	// grid
	iVal = mpKSPlugIn->GetGlobalParm(giParamID_Grid, giSectionGUI);
	TiXmlElement* pGrid = new TiXmlElement( "grid" );
	iVal ? sprintf(pszBuff, "on") : sprintf(pszBuff, "off");
	TiXmlText* pGridTxt = new TiXmlText(pszBuff);
	pGrid->LinkEndChild( pGridTxt );
	pEditing->LinkEndChild( pGrid );
	
	// snap
	iVal = mpKSPlugIn->GetGlobalParm(giParamID_KS_Snap_To, giSectionGlobal);
	TiXmlElement* pSnap = new TiXmlElement( "snap" );
	iVal ? sprintf(pszBuff, "on") : sprintf(pszBuff, "off");
	TiXmlText* pSnapTxt = new TiXmlText(pszBuff);
	pSnap->LinkEndChild( pSnapTxt );
	pEditing->LinkEndChild( pSnap );
	
	// waves
	iVal = mpKSPlugIn->GetGlobalParm(giParamID_Show_Waveform, giSectionGUI);
	TiXmlElement* pWaves = new TiXmlElement( "waves" );
	iVal ? sprintf(pszBuff, "on") : sprintf(pszBuff, "off");
	TiXmlText* pWavesTxt = new TiXmlText(pszBuff);
	pWaves->LinkEndChild( pWavesTxt );
	pEditing->LinkEndChild( pWaves );
	
	// fades
	iVal = mpKSPlugIn->GetGlobalParm(giParamID_Show_Fade, giSectionGUI);
	TiXmlElement* pFades = new TiXmlElement( "fades" );
	iVal ? sprintf(pszBuff, "on") : sprintf(pszBuff, "off");
	TiXmlText* pFadesTxt = new TiXmlText(pszBuff);
	pFades->LinkEndChild( pFadesTxt );
	pEditing->LinkEndChild( pFades );
	
	// loop
	TiXmlElement* pLoop = new TiXmlElement( "loop" );;
	pEditing->LinkEndChild( pLoop );
	Write_Loop(pLoop);
	
/*	// position
	iVal = mpKSPlugIn->GetGlobalParm(giParamID_Loop_End, giSectionGlobal);
	sprintf(pszBuff, "%d", iVal);
	TiXmlElement* pEnd = new TiXmlElement( "end" );
	TiXmlText* pEndTxt = new TiXmlText(pszBuff);
	pEnd->LinkEndChild( pEndTxt );
	pParent->LinkEndChild( pEnd );
*/	
	// windows
	TiXmlElement* pWindows = new TiXmlElement( "windows" );;
	pEditing->LinkEndChild( pWindows );
	Write_Windows(pWindows);
	
	
}

void CKSXML_Write::Write_Tool(TiXmlElement* pParent)
{
	char pszBuff [64];
	tuint uiTest	=	 mpKSPlugIn->GetGlobalParm(giParamID_Tool_Selected, giSectionGUI);
	
	
	switch(uiTest){
			
		case giTool_Hand:{
			sprintf(pszBuff, "hand");
			break;
		}
		case giTool_Trim:{
			sprintf(pszBuff, "trim");
			break;
		}
		case giTool_Select:{
			sprintf(pszBuff, "select");
			break;
		}
		case giTool_Cut:{
			sprintf(pszBuff, "cut");
			break;
		}
	}
	
	// text
	TiXmlText* pToolTxt = new TiXmlText(pszBuff);
	pParent->LinkEndChild( pToolTxt );
}

void CKSXML_Write::Write_Loop(TiXmlElement* pParent)
{
	char pszBuff [64];

	// on
	tint32 iVal = mpKSPlugIn->GetGlobalParm(giParamID_Loop_On, giSectionGlobal);
	TiXmlElement* pActive = new TiXmlElement( "active" );
	iVal ? sprintf(pszBuff, "on") : sprintf(pszBuff, "off");
	TiXmlText* pActiveTxt = new TiXmlText(pszBuff);
	pActive->LinkEndChild( pActiveTxt );
	pParent->LinkEndChild( pActive );
	
	// start
	iVal = mpKSPlugIn->GetGlobalParm(giParamID_Loop_Start, giSectionGlobal);
	sprintf(pszBuff, "%d", iVal);
	TiXmlElement* pStart = new TiXmlElement( "start" );
	TiXmlText* pStartTxt = new TiXmlText(pszBuff);
	pStart->LinkEndChild( pStartTxt );
	pParent->LinkEndChild( pStart );
	
	// end
	iVal = mpKSPlugIn->GetGlobalParm(giParamID_Loop_End, giSectionGlobal);
	sprintf(pszBuff, "%d", iVal);
	TiXmlElement* pEnd = new TiXmlElement( "end" );
	TiXmlText* pEndTxt = new TiXmlText(pszBuff);
	pEnd->LinkEndChild( pEndTxt );
	pParent->LinkEndChild( pEnd );
	
	
}

void CKSXML_Write::Write_Windows(TiXmlElement* pParent)
{
	// edit
	TiXmlElement* pEdit = new TiXmlElement( "edit" );;
	pParent->LinkEndChild( pEdit );
	Write_Window_Edit(pEdit);
	
	// mix
	TiXmlElement* pMix = new TiXmlElement( "mix" );;
	pParent->LinkEndChild( pMix );
	Write_Window_Mix(pMix);
	
	// rack
	TiXmlElement* pRack = new TiXmlElement( "rack" );;
	pParent->LinkEndChild( pRack );
	Write_Window_Rack(pRack);
}

void CKSXML_Write::Write_Window_Edit(TiXmlElement* pParent)
{
	
//	sprintf(pszBuff, "%d", iVal);
	
	// show
	TiXmlElement* pShow = new TiXmlElement( "show" );
	TiXmlText* pShowTxt = new TiXmlText("on");
	pShow->LinkEndChild( pShowTxt );
	pParent->LinkEndChild( pShow );
	
	
	// layer
	char pszBuff [64];
	tint32 iVal = 1;//mpKSPlugIn->GetGlobalParm(giParamID_Loop_On, giSectionGlobal);
	sprintf(pszBuff, "%d", iVal);
	TiXmlElement* pLayer = new TiXmlElement( "layer" );
	TiXmlText* pLayerTxt = new TiXmlText(pszBuff);
	pLayer->LinkEndChild( pLayerTxt );
	pParent->LinkEndChild( pLayer );
	
	// position
	TiXmlElement* pPosition = new TiXmlElement( "position" );;
	pParent->LinkEndChild( pPosition );
	Write_Window_Position(pPosition, 100, 100);
	
	// size
	TiXmlElement* pSize = new TiXmlElement( "size" );;
	pParent->LinkEndChild( pSize );
	Write_Window_Size(pSize, 800, 600);
	Add_Comment(pParent, "the order of tracks is considered an editing setting, and is specified here");
	
}

void CKSXML_Write::Write_Window_Mix(TiXmlElement* pParent)
{
	
}

void CKSXML_Write::Write_Window_Rack(TiXmlElement* pParent)
{
	
}

void CKSXML_Write::Write_Window_Position(TiXmlElement* pParent, tuint uiPosX, tuint uiPosY)
{
	char pszBuff [64];
	
	// x
	sprintf(pszBuff, "%d", uiPosX);
	TiXmlElement* pX = new TiXmlElement( "x" );
	TiXmlText* pXTxt = new TiXmlText(pszBuff);
	pX->LinkEndChild( pXTxt );
	pParent->LinkEndChild( pX );
	
	// y
	sprintf(pszBuff, "%d", uiPosY);
	TiXmlElement* pY = new TiXmlElement( "y" );
	TiXmlText* pYTxt = new TiXmlText(pszBuff);
	pY->LinkEndChild( pYTxt );
	pParent->LinkEndChild( pY );
	
}
	
void CKSXML_Write::Write_Window_Size(TiXmlElement* pParent, tuint uiSizeX, tuint uiSizeY)
{
	char pszBuff [64];
	
	// x
	sprintf(pszBuff, "%d", uiSizeX);
	TiXmlElement* pX = new TiXmlElement( "x" );
	TiXmlText* pXTxt = new TiXmlText(pszBuff);
	pX->LinkEndChild( pXTxt );
	pParent->LinkEndChild( pX );
	
	// y
	sprintf(pszBuff, "%d", uiSizeY);
	TiXmlElement* pY = new TiXmlElement( "y" );
	TiXmlText* pYTxt = new TiXmlText(pszBuff);
	pY->LinkEndChild( pYTxt );
	pParent->LinkEndChild( pY );
	
}

//----------------------------------------------------------------
// samples
//----------------------------------------------------------------
void CKSXML_Write::Write_Samples(TiXmlElement* pParent)
{
	Add_Comment(pParent, "samples and their takes. only used takes are listed. a sample can be included that's not used on any track");
	
		
	std::list<CKSPlugIn::SFileInfo*> sFileInfos = mpKSPlugIn->GetFileInfo();
	
	

	std::list<CKSPlugIn::SFileInfo*>::iterator  itFileList = sFileInfos.begin();
	for (; itFileList != sFileInfos.end(); itFileList++) {
		
		TiXmlElement* pName = new TiXmlElement( "sample" );
		
		
		CKSPlugIn::SFileInfo* pInfo = *itFileList;
	//	pInfo->sOriginalName.c_str();
		
		
		TiXmlText* pSampleTxt = new TiXmlText(pInfo->sOriginalName.c_str());
		
		pName->LinkEndChild( pSampleTxt );
		pParent->LinkEndChild( pName );
		
		
		
		
		
	}
	
}

void CKSXML_Write::Write_Sample(TiXmlElement* pParent)
{
	
	
	
	printf(".");
	
}

//----------------------------------------------------------------
// tracks bus and master
//----------------------------------------------------------------

void CKSXML_Write::Write_Tracks(TiXmlElement* pParent)
{
	tint32 iNrTracks = mpKSPlugIn->Get_Number_Of_Tracks();//mpKSPlugIn->msStack.iNr_Of_Tracks;
	
	for(tint32 i = 0; i<iNrTracks; i++){
		
		// ID
		tuint uiTrack = mpKSPlugIn->Get_Track_Id(i);
		// track
		TiXmlElement* pTrack = new TiXmlElement( "track" );
		pTrack->SetAttribute("id",uiTrack);
		pParent->LinkEndChild( pTrack );
		// write track data
		Write_Track(pTrack, uiTrack);	
		
	}
}

void CKSXML_Write::Write_Track(TiXmlElement* pParent, tuint uiTrack)
{
	// name
	std::string str  = mpKSPlugIn->GetChannelName(uiTrack);
	TiXmlElement* pName = new TiXmlElement( "name" );
	TiXmlText* pNameTxt = new TiXmlText(str.c_str());
	pName->LinkEndChild( pNameTxt );
	pParent->LinkEndChild( pName );
	 
	// description
	TiXmlElement* pDescription = new TiXmlElement( "description" );
	TiXmlText* pDescriptionTxt = new TiXmlText("NA");
	pDescription->LinkEndChild( pDescriptionTxt );
	pParent->LinkEndChild( pDescription );
	
	// in
	TiXmlElement* pIn = new TiXmlElement( "in" );
	pParent->LinkEndChild( pIn );
	Write_Track_In(pIn, uiTrack);
	
	// out
	TiXmlElement* pOut = new TiXmlElement( "out" );
	pParent->LinkEndChild( pOut );
	Write_Track_Out(pOut, uiTrack);
	
	// inserts
	Write_Inserts(pParent, uiTrack);
	
	// AUXs
	Write_AUXs(pParent, uiTrack);
	
	
}

void CKSXML_Write::Write_Track_In(TiXmlElement* pParent, tuint uiID)
{
	
	// input
	char pszBuff [64];
	tint32 iVal = mpKSPlugIn->GetGlobalParm(giParam_ChIn, giSection_First_Track + uiID);
	sprintf(pszBuff, "%d", iVal);
	TiXmlElement* pIn = new TiXmlElement( "input" );
	TiXmlText* pInTxt = new TiXmlText(pszBuff);
	pIn->LinkEndChild( pInTxt );
	pParent->LinkEndChild( pIn );
	
	// mode
	iVal = mpKSPlugIn->GetGlobalParm(giParam_ChInMode, giSection_First_Track + uiID);
	iVal ? sprintf(pszBuff, "mono") : sprintf(pszBuff, "stereo");
	TiXmlElement* pMode = new TiXmlElement( "in" );
	TiXmlText* pModeTxt = new TiXmlText(pszBuff);
	pMode->LinkEndChild( pModeTxt );
	pParent->LinkEndChild( pMode );
	
	// gain
	iVal = mpKSPlugIn->GetGlobalParm(giParam_ChInGain, giSection_First_Track + uiID);
	tfloat fGain	=	(tfloat)iVal * 0.0001f;
	sprintf(pszBuff, "%f", fGain);
	TiXmlElement* pGain = new TiXmlElement( "gain" );
	TiXmlText* pGainTxt = new TiXmlText(pszBuff);
	pGain->LinkEndChild( pGainTxt );
	pParent->LinkEndChild( pGain );
	
}

void CKSXML_Write::Write_Track_Out(TiXmlElement* pParent, tuint uiTrack)
{
	
	// output
	char pszBuff [64];
	tint32 iVal = mpKSPlugIn->GetGlobalParm(giParam_ChOut, giSection_First_Track + uiTrack);
	
	if(iVal < ParmIOOffsetBus) {
		sprintf(pszBuff, "mix", iVal);
	}
	else{
		sprintf(pszBuff, "bus %d", iVal-ParmIOOffsetBus);
	}
	/*
	TiXmlElement* pOut = new TiXmlElement( "out" );
	TiXmlText* pOutTxt = new TiXmlText(pszBuff);
	pOut->LinkEndChild( pOutTxt );
	pParent->LinkEndChild( pOut );
	*/
	// gain
	iVal = mpKSPlugIn->GetGlobalParm(giParam_ChVol, giSection_First_Track + uiTrack);
	tfloat fVolume	=	(tfloat)iVal * 0.0001f;
	sprintf(pszBuff, "%f", fVolume);
	TiXmlElement* pVolume = new TiXmlElement( "volume" );
	TiXmlText* pVolumeTxt = new TiXmlText(pszBuff);
	pVolume->LinkEndChild( pVolumeTxt );
	pParent->LinkEndChild( pVolume );
	
	// pan
	iVal = mpKSPlugIn->GetGlobalParm(giParam_ChPannerLeftRight, giSection_First_Track + uiTrack);
	tfloat fPan	=	(tfloat)iVal * 0.0001f;
	sprintf(pszBuff, "%f", fPan);
	TiXmlElement* pPan = new TiXmlElement( "pan" );
	TiXmlText* pPanTxt = new TiXmlText(pszBuff);
	pPan->LinkEndChild( pPanTxt );
	pParent->LinkEndChild( pPan );
	
	// solo
	iVal = mpKSPlugIn->GetGlobalParm(giParam_ChSolo, giSection_First_Track + uiTrack);
	iVal ? sprintf(pszBuff, "on") : sprintf(pszBuff, "off");
	TiXmlElement* pSolo = new TiXmlElement( "solo" );
	TiXmlText* pSoloTxt = new TiXmlText(pszBuff);
	pSolo->LinkEndChild( pSoloTxt );
	pParent->LinkEndChild( pSolo );
	
	// mute
	iVal = mpKSPlugIn->GetGlobalParm(giParam_ChMute, giSection_First_Track + uiTrack);
	iVal ? sprintf(pszBuff, "on") : sprintf(pszBuff, "off");
	TiXmlElement* pMute = new TiXmlElement( "mute" );
	TiXmlText* pMuteTxt = new TiXmlText(pszBuff);
	pMute->LinkEndChild( pMuteTxt );
	pParent->LinkEndChild( pMute );
	
	// arm
	iVal = mpKSPlugIn->GetGlobalParm(giParam_ChArm, giSection_First_Track + uiTrack);
	iVal ? sprintf(pszBuff, "on") : sprintf(pszBuff, "off");
	TiXmlElement* pArm = new TiXmlElement( "arm" );
	TiXmlText* pArmTxt = new TiXmlText(pszBuff);
	pArm->LinkEndChild( pArmTxt );
	pParent->LinkEndChild( pArm );
}

void CKSXML_Write::Write_Inserts(TiXmlElement* pParent, tuint uiTrack)
{
	
	for(tuint uiInsert=0; uiInsert<giNumber_Of_Inserts; uiInsert++)
	{
		Write_Insert( pParent, uiTrack, uiInsert);
	}
}

void CKSXML_Write::Write_Insert(TiXmlElement* pParent, tuint uiTrack, tuint uiInsert)
{
	
	// output
//	char pszBuff [64];
	tint32 iInsertId = mpKSPlugIn->GetGlobalParm(giParam_ChInsert1 + uiInsert, giSection_First_Track + uiTrack);
	
	if(iInsertId){
		
		
		// in
		TiXmlElement* pInsert = new TiXmlElement( "insert" );
		pParent->LinkEndChild( pInsert );
		pInsert->SetAttribute("slot",uiInsert);
		pInsert->SetAttribute("id", iInsertId);
		
		CPlugInManager* pPlugManager = mpKSPlugIn->GetPlugInManager();
		//CPlugInManager::SPlugInInfo* pInfo = pPlugManager->GetPlugInInfo(0);
		
		std::string s = pPlugManager->GetPlugInVendor(iInsertId);
		std::string u = pPlugManager->GetPlugInName(iInsertId);
		
		// vendor
		TiXmlElement* pVendor = new TiXmlElement( "vendor" );
		TiXmlText* pVendorTxt = new TiXmlText(s.c_str());
		pVendor->LinkEndChild( pVendorTxt );
		pInsert->LinkEndChild( pVendor );
		
		// product
		TiXmlElement* pProduct = new TiXmlElement( "product" );
		TiXmlText* pProductTxt = new TiXmlText(u.c_str());
		pProduct->LinkEndChild( pProductTxt );
		pInsert->LinkEndChild( pProduct );
		
		/*
		// Get number of parameters here
		// tint iParameters = pPlugManager->GetPlugInInfo(iInsertId);
		CChannel* pChannel = dynamic_cast<CKSDSP*>(mpKSPlugIn->GetDSPEngine())->GetChannel(uiTrack);
		kspi::IPlugIn* pPlugIn = pChannel->GetInsert(uiInsert);
		
		tint32 iSize = pPlugIn->GetChunkSize();
		tchar* p = new tchar[iSize];
		//tint* p = new tint[iSize];
		pPlugIn->GetChunk(p);
		
		for(tuint i =0; i<iSize; i++){
			printf("%d,", p[i]);
		}
		delete[] p;
		printf("\n");			
		*/	
		
		
		
		

	}
}

void CKSXML_Write::Write_AUXs(TiXmlElement* pParent, tuint uiTrack)
{
	// prepared for more than two AUX sends
	for(tuint iAUX=0; iAUX<giNumber_Of_AUXes; iAUX++)
	{
		Write_AUX( pParent, uiTrack, iAUX);
	}
}

void CKSXML_Write::Write_AUX(TiXmlElement* pParent, tuint uiTrack, tuint iAux)
{
		
	tint32 iSend = mpKSPlugIn->GetGlobalParm(giParam_ChAUX1 + iAux, giSection_First_Track + uiTrack);
	
	if(iSend){
		
		char pszBuff [64];
		// aux
		TiXmlElement* pAux = new TiXmlElement( "aux" );
		pParent->LinkEndChild( pAux );
		pAux->SetAttribute("id",iAux);
		
		// send
		tfloat fVolume	=	(tfloat)iSend * 0.0001f;
		sprintf(pszBuff, "%f", fVolume);
		TiXmlText* pAUXTxt = new TiXmlText(pszBuff);
		pAux->LinkEndChild( pAUXTxt );

				
	}
/*	
	// output
	char pszBuff [64];
	tint32 iSend = mpKSPlugIn->GetGlobalParm(giParam_ChAUX1 + iAux, giSection_First_Track + uiTrack);
	
	if(iSend){
		
		TiXmlElement* pAux = new TiXmlElement( "aux" );
		pAux->LinkEndChild( pParent );
		pAux->SetAttribute("id",iAux);
		
		// gain
		tfloat fVolume	=	(tfloat)iSend * 0.0001f;
		sprintf(pszBuff, "%f", fVolume);
		TiXmlElement* pSend = new TiXmlElement( "send" );
		TiXmlText* pSendTxt = new TiXmlText(pszBuff);
		pSend->LinkEndChild( pSendTxt );
		pAux->LinkEndChild( pSend );
		
	}
 */
}

void CKSXML_Write::Write_Busses(TiXmlElement* pParent)
{
//	tint32 iNrTracks = mpKSPlugIn->Get_Number_Of_Tracks();//mpKSPlugIn->msStack.iNr_Of_Tracks;
	
	for(tint32 i = 0; i<giNumber_Of_Busses; i++){
		
		// ID
	//	tuint uiID = mpKSPlugIn->Get_Track_Id(i);
		// track
		TiXmlElement* pBus = new TiXmlElement( "bus" );
		pBus->SetAttribute("id",i);
		pParent->LinkEndChild( pBus );
		// write track data
		Write_Bus(pBus, i);	
		
	}
}

void CKSXML_Write::Write_Bus(TiXmlElement* pParent, tuint uiID)
{
	// out
	TiXmlElement* pOut = new TiXmlElement( "out" );
	pParent->LinkEndChild( pOut );
	Write_Bus_Out(pOut, uiID);
	
}

void CKSXML_Write::Write_Bus_Out(TiXmlElement* pParent, tuint uiID)
{
	
	// output
	char pszBuff [64];
	tint32 iVal = mpKSPlugIn->GetGlobalParm(giParam_Buss_Out, giSection_First_Buss + uiID);
	
	if(iVal < ParmIOOffsetBus) {
		sprintf(pszBuff, "mix", iVal);
	}
	else{
		sprintf(pszBuff, "bus %d", iVal-ParmIOOffsetBus);
	}
	TiXmlElement* pOut = new TiXmlElement( "out" );
	TiXmlText* pOutTxt = new TiXmlText(pszBuff);
	pOut->LinkEndChild( pOutTxt );
	pParent->LinkEndChild( pOut );
	
	// gain
	iVal = mpKSPlugIn->GetGlobalParm(giParam_Buss_Vol, giSection_First_Buss + uiID);
	tfloat fVolume	=	(tfloat)iVal * 0.0001f;
	sprintf(pszBuff, "%f", fVolume);
	TiXmlElement* pVolume = new TiXmlElement( "volume" );
	TiXmlText* pVolumeTxt = new TiXmlText(pszBuff);
	pVolume->LinkEndChild( pVolumeTxt );
	pParent->LinkEndChild( pVolume );
	
	// pan
	iVal = mpKSPlugIn->GetGlobalParm(giParam_Buss_PannerLeftRight, giSection_First_Buss + uiID);
	tfloat fPan	=	(tfloat)iVal * 0.0001f;
	sprintf(pszBuff, "%f", fPan);
	TiXmlElement* pPan = new TiXmlElement( "pan" );
	TiXmlText* pPanTxt = new TiXmlText(pszBuff);
	pPan->LinkEndChild( pPanTxt );
	pParent->LinkEndChild( pPan );
	
	// solo
	iVal = mpKSPlugIn->GetGlobalParm(giParam_Buss_Solo, giSection_First_Buss + uiID);
	iVal ? sprintf(pszBuff, "on") : sprintf(pszBuff, "off");
	TiXmlElement* pSolo = new TiXmlElement( "solo" );
	TiXmlText* pSoloTxt = new TiXmlText(pszBuff);
	pSolo->LinkEndChild( pSoloTxt );
	pParent->LinkEndChild( pSolo );
	
	// mute
	iVal = mpKSPlugIn->GetGlobalParm(giParam_Buss_Mute, giSection_First_Buss + uiID);
	iVal ? sprintf(pszBuff, "on") : sprintf(pszBuff, "off");
	TiXmlElement* pMute = new TiXmlElement( "mute" );
	TiXmlText* pMuteTxt = new TiXmlText(pszBuff);
	pMute->LinkEndChild( pMuteTxt );
	pParent->LinkEndChild( pMute );
	
}
//----------------------------------------------------------------
// comments
//----------------------------------------------------------------
void CKSXML_Write::Add_Comment(TiXmlDocument* pDoc, std::string str)
{
	
	//	TiXmlDeclaration* decl = new TiXmlDeclaration( s.c_str() );
	TiXmlComment*	comment = new TiXmlComment(str.c_str() );
	pDoc->LinkEndChild( comment );
	
}

void CKSXML_Write::Add_Comment( TiXmlElement* pParent, std::string str)
{
	
	//	TiXmlDeclaration* decl = new TiXmlDeclaration( s.c_str() );
	TiXmlComment*	comment = new TiXmlComment(str.c_str() );
	pParent->LinkEndChild( comment );
	
}
