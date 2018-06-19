#include "Includes.h"
#include "DoFunction.h"

/*
**
** DoFunction()
**
*/

int DoFunction( int Operation, int side, char * Argument )
{
	APTR wi_Information, wi_Protection, tx_TextLeft, tx_TextRight, bt_Okay, bt_Cancel, bt_All;
	APTR cy_Hide, cy_Script, cy_Pure, cy_Archive, cy_Read, cy_Write, cy_Execute, cy_Delete;
	BPTR fh, lock;
	__aligned struct FileInfoBlock * fib;
	struct DateStamp * date;
	struct DateTime * datetime;
	ULONG i, k = 0, max, Entries_ULONG, Selection_State, Opened, Iconified, signal;
	LONG h,s,p,a,r,w,e,d, Set_LONG, Visible_LONG, Active_LONG, First_LONG, Diff_LONG, FirstSelected_LONG = -1, Over, DelForce, Proceed;
	const char * CY_Protect[] = { "X", "X", "X", NULL };
	char FileName_String[512], String[1024], SrcDevice_String[512], DstDevice_String[512], Target_String[512];
	char Date_String[16], Time_String[10], ProtBits[10], Type_String[51], Size_String[21], Version_String[81];
	char Help_String_A[512], Help_String_B[512];
	char * ptrA = NULL, * NewName_String, * cptr;
	BOOL Action_BOOL = FALSE, ReloadSrc_BOOL = FALSE, Skip = FALSE, Cancel = FALSE, success, All_BOOL = FALSE, Request_Dst = FALSE;
	BOOL Fitting_BOOL = TRUE, Move_BOOL = FALSE, As_BOOL = FALSE, SameDevice_BOOL = FALSE, Done;
	BOOL ReloadDst_BOOL = FALSE, NoBreak_BOOL = FALSE, NoDeselect_BOOL = FALSE, Jumped = FALSE, CommentAll_BOOL = FALSE;
	int ErrorNum = 0, rc, l, TypeNum = -1;

	if ( Argument != NULL )
	{
		ParseMisc( Argument, &NoBreak_BOOL, &NoDeselect_BOOL );
		ParseReload( Argument, &ReloadSrc_BOOL, &ReloadDst_BOOL );
		ParseNeedTarget( side, Argument, &Request_Dst );
	}

	if ( Operation == command_entries && Request_Dst )
	{
		ptrA = strstr( Argument, "%f" );
		if ( ptrA == NULL )
			ptrA = strstr( Argument, "%F" );
			if ( ptrA == NULL )
				ptrA = strstr( Argument, "%n" );
				if ( ptrA == NULL )
					ptrA = strstr( Argument, "%N" );

		if ( ptrA == NULL )
		{
			rc = ExecuteCommand( Argument, "" );
			if ( rc != 0 && !NoBreak_BOOL)
				ErrorNum = -4;
		}
	}

	if ( ( Operation != command_entries) || ( ptrA != NULL ) )
	{
		if ( global_DirLoaded[side] )
		{
			Over = cfg_Overwrite;
			DelForce = cfg_DelForce;

			get( lv_Directory[side], MUIA_List_Active, &Active_LONG );
			get( lv_Directory[side], MUIA_List_Entries, &Entries_ULONG );
			for ( i = 0; i < Entries_ULONG; i++ )
			{
				DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );
				if ( Selection_State == MUIV_List_Select_On )
				{
					k++;
					if ( FirstSelected_LONG == -1 )
						FirstSelected_LONG = i;
				}
			}

			if ( k == 0 && Active_LONG != MUIV_List_Active_Off)
			{
				k = 1;
				DoMethod( lv_Directory[side], MUIM_List_Select, Active_LONG, MUIV_List_Select_On, NULL );
			}

			set( ga_Gauge, MUIA_Gauge_Current, 0 );
			set( ga_Gauge, MUIA_Gauge_Max, k );


			if ( k > 0 )
			{
				k = 0;
				set( wi_Progress, MUIA_Window_Open, TRUE );

				if ( cfg_Scroll )
				{
					get( lv_Directory[side], MUIA_List_Visible, &Visible_LONG );
					get( lv_Directory[side], MUIA_List_First, &First_LONG );

					if ( FirstSelected_LONG >= First_LONG )
						Diff_LONG = FirstSelected_LONG - First_LONG;
					else
					{
						Diff_LONG = Visible_LONG + 1;
						First_LONG = FirstSelected_LONG;
					}
					if ( Diff_LONG > Visible_LONG )
					{
						set( lv_Directory[side], MUIA_List_Quiet, TRUE );
						DoMethod( lv_Directory[side], MUIM_List_Jump, Entries_ULONG - 1 );
						DoMethod( lv_Directory[side], MUIM_List_Jump, FirstSelected_LONG );
					}
				}

				for ( i = 0; i < Entries_ULONG; i++ )
				{
					if ( ( cfg_Scroll ) && ( ( ( i - First_LONG ) % Visible_LONG ) == 0 ) && i > First_LONG )
					{
						set( lv_Directory[side], MUIA_List_Quiet, TRUE );
						if ( ( i + Visible_LONG ) < Entries_ULONG )
							DoMethod( lv_Directory[side], MUIM_List_Jump, i - 1 + Visible_LONG );
						else
							DoMethod( lv_Directory[side], MUIM_List_Jump, Entries_ULONG - 1 );
						Jumped = TRUE;
					}

					DoMethod( app_RumorOpus, MUIM_Application_InputBuffered );
					DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );

					if ( Selection_State == MUIV_List_Select_On )
					{
						if ( cfg_Scroll )
							set( lv_Directory[side], MUIA_List_Quiet, FALSE );

						get( wi_Progress, MUIA_Window_Open, &Opened );
						get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
						if ( ( !Opened ) && ( !Iconified ) ) break;

						DoMethod( lv_Directory[side], MUIM_List_GetEntry, i, &fib );
						sprintf( FileName_String, "%s%s", GetPath( side ), fib -> fib_FileName );
						strcpy( String, "" );

						switch ( Operation )
						{
							case command_entries:
								if ( Request_Dst )
								{
									sprintf( Status_String, GetCatStr( 46, "Executing Command for '%s'..." ), FileName_String );
									set( bt_StatusBar, MUIA_Text_Contents, Status_String );
									rc = ExecuteCommand( Argument, FileName_String );
									if ( rc != 0 && !NoBreak_BOOL)
										ErrorNum = -4;
									Action_BOOL = TRUE;
								}
								else
									Cancel = TRUE;
								break;

							case action_entries:
								if ( fib -> fib_DirEntryType < 0 )
								{
									sprintf( Status_String, GetCatStr( 47, "Performing FileAction for '%s'..." ), FileName_String );
									set( bt_StatusBar, MUIA_Text_Contents, Status_String );
									ErrorNum = FileAction( FileName_String );
									if ( ErrorNum == -4 && NoBreak_BOOL)
										ErrorNum = 0;
								}
								break;

							case copydev_entries:
								sprintf( Status_String, GetCatStr( 48, "Copying '%s'..." ), FileName_String );
								set( bt_StatusBar, MUIA_Text_Contents, Status_String );
								if ( !Action_BOOL )
								{
									NewName_String = StringRequester( GetCatStr( 49, "Copy selected files to device" ), "PRT:", "/", 31, 0, &Skip, &Cancel );
									if ( ! ( strlen( NewName_String ) > 0 ) )
										Cancel = TRUE;
									else
										strcpy( Target_String, NewName_String );
								}
								if ( !Skip && !Cancel )
									ErrorNum = Copy( FileName_String, Target_String );
								if ( ErrorNum == -20 )
									ErrorNum = 0;
								break;

							case delete_entries:
								if ( cfg_DelVerify && !Action_BOOL )
								{
									rc = MUI_Request( app_RumorOpus, wi_Main, 0, GetCatStr( 50, "Request") , GetCatStr( 51, "*_Delete|_Cancel" ), GetCatStr( 52, "Are you sure you want\nto delete these entries?" ), TAG_DONE );
									if ( rc == 0 ) Cancel = TRUE;
								}
								if ( !Cancel )
								{
									sprintf( Status_String, GetCatStr( 53, "Deleting '%s'..." ), FileName_String );
									set( bt_StatusBar, MUIA_Text_Contents, Status_String );
									ErrorNum = Delete( FileName_String );
									if ( ErrorNum == 0 )
										ReloadSrc_BOOL = TRUE;
								}
								break;

							case unarc_entries:
								if ( fib -> fib_DirEntryType < 0 && global_DirLoaded[OtherSide(side)] )
								{
									sprintf( Status_String, GetCatStr( 77, "Extracting '%s' to '%s'..." ), FileName_String, GetPath( OtherSide( side ) ) );
									set( bt_StatusBar, MUIA_Text_Contents, Status_String );
									TypeNum = RecogArchive( FileName_String );
									if ( TypeNum != -1 )
										strcpy( String, cfg_RecogCommandB[TypeNum] );
								}
								if ( strlen( String ) > 0 )
								{
									ParseMisc( Argument, &NoBreak_BOOL, &NoDeselect_BOOL );
									ParseReload( String, &ReloadSrc_BOOL, &ReloadDst_BOOL );
									ParseNeedTarget( side, String, &Request_Dst );
									if ( Request_Dst )
									{
										rc = ExecuteCommand( String, FileName_String );
										if ( rc != 0 && !NoBreak_BOOL)
											ErrorNum = -4;
										Action_BOOL = TRUE;
									}
									else
										Cancel = TRUE;
									break;
								}
								break;

							case drop_entries:
							case copy_entries:
							case move_entries:
							case copyas_entries:
							case moveas_entries:
								if ( global_DirLoaded[OtherSide(side)] )
								{
									if ( !Action_BOOL )
									{
										ReloadDst_BOOL = TRUE;

										strcpy( SrcDevice_String, GetPath( side ) );
										cptr = strrchr( SrcDevice_String, ':' );
										cptr++;
										* cptr = '\0';

										strcpy( DstDevice_String, GetPath( OtherSide( side ) ) );
										cptr = strrchr( DstDevice_String, ':' );
										cptr++;
										* cptr = '\0';

										if ( stricmp( DstDevice_String, SrcDevice_String ) == 0 )
											SameDevice_BOOL = TRUE;

										if ( Operation == move_entries || Operation == moveas_entries )
											Move_BOOL = TRUE;

										if ( cfg_CheckFit && ( ! ( SameDevice_BOOL && Move_BOOL ) ) )
										{
											if ( cfg_PathExpand )
											{
												lock = Lock( "RAM:", ACCESS_READ );
												if ( lock )
												{
													success = NameFromLock( lock, String, sizeof( String ) );
													if ( !success )
														ErrorNum = IoErr();
													UnLock( lock );
												}
												else
													ErrorNum = IoErr();
											}
											else
												strcpy( String, "RAM:" );

											if ( ErrorNum == 0 )
											{
												if ( strstr( GetPath( OtherSide( side ) ), String ) == NULL )
													if( strstr( Fit( side, FALSE ), GetCatStr( 54, "NO!" ) ) != NULL )
														Fitting_BOOL = FALSE;
											}
										}
										if ( Operation == copyas_entries || Operation == moveas_entries )
											As_BOOL = TRUE;
									}

									get( wi_Progress, MUIA_Window_Open, &Opened );
									get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );

									if ( ErrorNum == 0 && ( Opened || Iconified ) )
									{
										if ( Fitting_BOOL || ( SameDevice_BOOL && Move_BOOL ) )
										{
											if ( !As_BOOL )
												sprintf( Status_String, GetCatStr( 55, "Copying '%s' to '%s%s'..." ), FileName_String, GetPath( OtherSide( side ) ), FilePart( FileName_String ) );
											else
												sprintf( Status_String, GetCatStr( 56, "Copying '%s'..." ), FileName_String );
											set( bt_StatusBar, MUIA_Text_Contents, Status_String );

											if ( As_BOOL )
											{
												if ( Operation == copyas_entries )
													sprintf( String, GetCatStr( 57, "Copy '%s' as" ), FilePart( FileName_String ) );
												else
													sprintf( String, GetCatStr( 58, "Move '%s' as" ), FilePart( FileName_String ) );
												NewName_String = StringRequester( String, FilePart( FileName_String ), ":/", 31, 1, &Skip, &Cancel );
												strcpy( String, NewName_String );
											}
											else
												strcpy( String, FilePart( FileName_String ) );

											if ( !Skip && !Cancel && strlen( String ) > 0 )
											{
												sprintf( Target_String, "%s%s", GetPath( OtherSide ( side ) ), String );
												if ( Move_BOOL && SameDevice_BOOL )
												{
													sprintf( Help_String_A, "/%s/", FileName_String );
													sprintf( Help_String_B, "/%s/", Target_String );
													if ( ( strstr( Help_String_B, Help_String_A ) == NULL ) && ( stricmp( FileName_String, Target_String ) != 0 ) )
													{
														if ( Exists( Target_String ) )
														{
															Proceed = Overwrite( FileName_String, Target_String );
															if ( Proceed == 1 )
																ErrorNum = Delete( Target_String );
															if ( Proceed == 0 )
															{
																ErrorNum = -20;
																if ( cfg_Overwrite == 1 )
																	ErrorNum = -7;
															}
														}
														else
															Proceed = 1;

														if ( Proceed == 1 && ErrorNum == 0 )
														{
															success = Rename( FileName_String, Target_String );
															if ( !success )
																ErrorNum = IoErr();
															else
																ReloadSrc_BOOL = TRUE;
														}
													}
													else
														ErrorNum = -6;
												}
												else
												{
													ErrorNum = Copy( FileName_String, Target_String );
													if ( ErrorNum == 0 )
													{
														Action_BOOL = TRUE;
														if ( Move_BOOL )
														{
															if ( Exists( Target_String ) )
																ErrorNum = Delete( FileName_String );
															if ( ErrorNum == 0 )
																ReloadSrc_BOOL = TRUE;
														}
														else
															ErrorNum = 0;
													}
													if ( ErrorNum == -20 )
														ErrorNum = 0;
												}
											}
										}
										else
											ErrorNum = -10;
									}
									else
										Cancel = TRUE;
								}
								else
									Cancel = TRUE;
								break;

							case rename_entries:
								sprintf( Status_String, GetCatStr( 59, "Renaming '%s'..." ), FileName_String );
								set( bt_StatusBar, MUIA_Text_Contents, Status_String );
								sprintf( String, GetCatStr( 60, "Rename '%s' as" ), fib -> fib_FileName );
								NewName_String = StringRequester( String, fib -> fib_FileName, ":/", 31, 1, &Skip, &Cancel );
								if ( !Skip && !Cancel && strlen( NewName_String ) > 0 )
								{
									Done = TRUE;
									sprintf( Target_String, "%s%s", GetPath( side ), NewName_String );
									if ( stricmp( FileName_String, Target_String ) != 0 )
										if ( Exists( Target_String ) )
											if ( Overwrite( FileName_String, Target_String ) == 1 )
												ErrorNum = Delete( Target_String );
											else
												Done = FALSE;
									if ( ErrorNum == 0 && Done )
									{
										success = Rename( FileName_String, Target_String );
										if ( !success )
											ErrorNum = IoErr();
										else
											ReloadSrc_BOOL = TRUE;
									}
								}		
								break;

							case dup_entries:
								sprintf( Status_String, GetCatStr( 61, "Duplicating '%s'..." ), FileName_String );
								set( bt_StatusBar, MUIA_Text_Contents, Status_String );
								sprintf( String, GetCatStr( 62, "Duplicate '%s' as" ), fib -> fib_FileName );
								NewName_String = StringRequester( String, fib -> fib_FileName, ":/", 31, 1, &Skip, &Cancel );
								if ( !Skip && !Cancel && strlen( NewName_String ) > 0 )
								{
									Done = TRUE;
									sprintf( Target_String, "%s%s", GetPath( side ), NewName_String );
									if ( stricmp( FileName_String, Target_String ) != 0 )
										if ( Exists( Target_String ) )
											if ( Overwrite( FileName_String, Target_String ) == 1 )
												ErrorNum = Delete( Target_String );
											else
												Done = FALSE;
									if ( ErrorNum == 0 && Done )
									{
										ErrorNum = Copy( FileName_String, Target_String );
										if ( ErrorNum == 0 )
											ReloadSrc_BOOL = TRUE;
										if ( ErrorNum == -20 )
											ErrorNum = 0;
									}
								}		
								break;

							case touch_entries:
								date = malloc( sizeof( struct DateStamp ) );
								if ( date )
								{
									DateStamp( date );
									sprintf( Status_String, GetCatStr( 63, "Touching '%s'..." ), FileName_String );
									set( bt_StatusBar, MUIA_Text_Contents, Status_String );
									success = SetFileDate( FileName_String, date );
									if ( !success )
										ErrorNum = IoErr();
									else
										ReloadSrc_BOOL = TRUE;
									free( date );
								}
								break;

							case comment_entries:
								sprintf( Status_String, GetCatStr( 64, "Setting comment for '%s'..." ), FileName_String );
								set( bt_StatusBar, MUIA_Text_Contents, Status_String );
								if ( !CommentAll_BOOL )
								{
									sprintf( String, GetCatStr( 65, "Enter comment for '%s'" ), fib -> fib_FileName );
									NewName_String = StringRequester( String, fib -> fib_Comment, "", 80, 2, &Skip, &Cancel );
									strcpy( Version_String, NewName_String );
								}
								if ( Skip )
									CommentAll_BOOL = TRUE;
								if ( !Cancel || CommentAll_BOOL )
								{
									success = SetComment( FileName_String, Version_String );
									if ( !success )
										ErrorNum = IoErr();
									else
										ReloadSrc_BOOL = TRUE;
								}
								break;

							case setdate_entries:
								sprintf( Status_String, GetCatStr( 66, "Setting datestamp for '%s'..." ), FileName_String );
								set( bt_StatusBar, MUIA_Text_Contents, Status_String );

								datetime = malloc( sizeof( struct DateTime ) );
								if ( datetime )
								{
									datetime -> dat_Stamp   = fib -> fib_Date;
									datetime -> dat_Format  = FORMAT_DOS;
									datetime -> dat_Flags   = NULL;
									datetime -> dat_StrDay  = NULL;
									datetime -> dat_StrDate = Date_String;
									datetime -> dat_StrTime = Time_String;

									if ( DateToStr( datetime ) )
										sprintf( String, "%s %s", Date_String, Time_String );
									else
										ErrorNum = -2;

									free( datetime );
								}
								else
									ErrorNum = -1;

								if ( ErrorNum == 0 )
								{
									sprintf( Status_String, GetCatStr( 67, "Enter datestamp for '%s'" ), fib -> fib_FileName );
									NewName_String = StringRequester( Status_String, String, "", 80, 1, &Skip, &Cancel );
									if ( !Skip && !Cancel && strlen( NewName_String ) > 0 )
									{
										rc = sscanf( NewName_String, "%s %s", Date_String, Time_String );
										if ( rc == 2 )
										{
											datetime = malloc( sizeof( struct DateTime ) );
											if ( datetime )
											{
												datetime -> dat_Format  = FORMAT_DOS;
												datetime -> dat_Flags   = NULL;
												datetime -> dat_StrDay  = NULL;
												datetime -> dat_StrDate = Date_String;
												datetime -> dat_StrTime = Time_String;

												if ( StrToDate( datetime ) )
												{
													success = SetFileDate( FileName_String, &datetime -> dat_Stamp );
													if(!success)
														ErrorNum = IoErr();
													else
														ReloadSrc_BOOL = TRUE;
												}
												else
													ErrorNum = -3;
											}
											else
												ErrorNum = -1;
											free(datetime);
										}
										else
											ErrorNum = -3;
									}
								}
								break;

							case info_entries:
								sprintf( Status_String, GetCatStr( 68, "Showing information for '%s'..." ), FileName_String );
								set( bt_StatusBar, MUIA_Text_Contents, Status_String );

								fib = malloc( sizeof( struct FileInfoBlock ) );
								if ( fib )
								{
									lock = Lock( FileName_String, ACCESS_READ );
									if ( lock )
									{
										Examine( lock, fib );
										datetime = malloc( sizeof( struct DateTime ) );
										if ( datetime )
										{
											datetime -> dat_Stamp   = fib -> fib_Date;
											datetime -> dat_Format  = FORMAT_DOS;
											datetime -> dat_Flags   = NULL;
											datetime -> dat_StrDay  = NULL;
											datetime -> dat_StrDate = Date_String;
											datetime -> dat_StrTime = Time_String;

											if ( ! ( DateToStr( datetime ) ) )
												ErrorNum = -2;

											if ( ErrorNum == 0 )
											{											
												for( l = 0; l < 8; l++ ) ProtBits[7-l] = ( fib -> fib_Protection^0x0f ) & ( 1 << l ) ? "dewrapsh"[l] : '-'; ProtBits[8]=0;
												if ( fib -> fib_DirEntryType > 0 )
													strcpy( Type_String, GetCatStr( 69, "Directory" ) );
												else
												{
													TypeNum = Recog( FileName_String );
													if ( TypeNum == -1 )
														TypeNum = RecogArchive( FileName_String );
													if ( TypeNum == -1 )
														strcpy( Type_String, "Binary File" );
													if ( TypeNum == -2 )
														strcpy( Type_String, "ASCII File" );
													if ( TypeNum == -3 )
														strcpy( Type_String, "Executable File" );
													if ( TypeNum == -4 )
														strcpy( Type_String, "XPK/PP Crunched File" );

													if ( TypeNum >= 0 )
														strcpy( Type_String, cfg_RecogName[TypeNum] );
												}
												strcpy(	Size_String, NumberToString( fib -> fib_Size ) );
												if ( TypeNum == -3 || TypeNum == -2 )
													strcpy( Version_String, GetVersion( FileName_String ) );
												else
													strcpy( Version_String, GetCatStr( 126, "unknown" ) );
												sprintf( String, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s", FileName_String, Date_String, Time_String, Size_String, ProtBits, Version_String, Type_String, fib -> fib_Comment );
											}

											free( datetime );
										}
										else
											ErrorNum = -1;
										UnLock( lock );
									}
									else
										ErrorNum = IoErr();
									free( fib );
								}
								else
									ErrorNum = -1;

								if ( ErrorNum == 0 )
								{

									wi_Information = WindowObject,
										MUIA_Window_ID, 6,
										MUIA_Window_Title, GetCatStr( 70, "File Information" ),
										MUIA_Window_Menu, MUIV_Window_Menu_NoMenu,
										WindowContents, VGroup,
											Child, HGroup, MUIA_Group_SameSize, FALSE,
												TextFrame,
												MUIA_Background, MUII_TextBack,
												Child, tx_TextLeft = TextObject,
													TextFrame,
													MUIA_FramePhantomHoriz, TRUE,
													MUIA_Background, MUII_TextBack,
													MUIA_Text_Contents, GetCatStr( 71, "Name:\nDate:\nTime:\nSize:\nStatus:\nVersion:\nDesc:\nNote:" ),
													MUIA_Text_SetMax, TRUE,
													End,
												Child, tx_TextRight = TextObject,
													TextFrame,
													MUIA_FramePhantomHoriz, TRUE,
													MUIA_Background, MUII_TextBack,
													MUIA_Text_Contents, String,
													End,
												End,
											Child, HGroup,
												MUIA_Group_SameSize, TRUE,
												Child, bt_Okay = SimpleButton(GetCatStr( 72, "_Okay" ) ),
												Child, HSpace(0),
												Child, HSpace(0),
												Child, bt_Cancel = SimpleButton(GetCatStr( 73, "_Cancel" ) ),
												End,
											End,
										End;

									if ( !wi_Information )
										Fail();

									DoMethod( wi_Information, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2 );
									DoMethod( bt_Okay, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 1 );
									DoMethod( bt_Cancel, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2 );
									DoMethod( app_RumorOpus, OM_ADDMEMBER, wi_Information );

									set( wi_Information, MUIA_Window_Open, TRUE );
									set( wi_Information, MUIA_Window_ActiveObject, bt_Okay);

									success = TRUE;
									while ( success )
									{
										switch ( DoMethod( app_RumorOpus, MUIM_Application_Input, &signal ) )
										{
											case MUIV_Application_ReturnID_Quit:
												global_QuitProgram = QuitRequester();
												if ( global_QuitProgram )
												{
													Cancel = TRUE;
													success = FALSE;
												}
												break;

											case 1:
												success = FALSE;
												break;

											case 2:
												Cancel = TRUE;
												success = FALSE;
												break;
										}
										if ( signal ) Wait( signal );
									}

									set( wi_Information, MUIA_Window_Open, FALSE );
									DoMethod( app_RumorOpus, OM_REMMEMBER, wi_Information );
									MUI_DisposeObject( wi_Information );
								}
								break;

							case listarc_entries:
								if ( fib -> fib_DirEntryType < 0 )
								{
									sprintf( Status_String, GetCatStr( 76, "Listing archive '%s'..." ), FileName_String );
									set( bt_StatusBar, MUIA_Text_Contents, Status_String );
									TypeNum = RecogArchive( FileName_String );
									if ( TypeNum != -1 )
										strcpy( String, cfg_RecogCommand[TypeNum] );
								}
								if ( strlen( String ) > 0 )
								{
									ParseMisc( Argument, &NoBreak_BOOL, &NoDeselect_BOOL );
									ParseReload( String, &ReloadSrc_BOOL, &ReloadDst_BOOL );
									ParseNeedTarget( side, String, &Request_Dst );
									if ( Request_Dst )
									{
										rc = ListArchive( String, FileName_String );
										if ( rc != 0 && !NoBreak_BOOL)
											ErrorNum = -4;
										Action_BOOL = TRUE;
									}
									else
										Cancel = TRUE;
								}
								else
									if ( !NoDeselect_BOOL )
										DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Off, NULL );
								break;

							case protect_entries:
								sprintf( Status_String, GetCatStr( 78, "Changing protection bits for '%s'..." ) , FileName_String );
								set( bt_StatusBar, MUIA_Text_Contents, Status_String );
								CY_Protect[0] = GetCatStr( 43, "Ignore" );
								CY_Protect[1] = GetCatStr( 44, "Set" );
								CY_Protect[2] = GetCatStr( 45, "UnSet" );
								if( !All_BOOL )
								{
									wi_Protection = WindowObject,
										MUIA_Window_ID, 7,
										MUIA_Window_Title, GetCatStr( 79, "Protection" ),
										MUIA_Window_Menu, MUIV_Window_Menu_NoMenu,
										WindowContents, VGroup,
											Child, HGroup, MUIA_Group_SameSize, TRUE,
												Child, ColGroup(2),
													Child, KeyLabel1(GetCatStr( 80, "Hide:" ),'h'), Child, cy_Hide = KeyCycle(CY_Protect,'h'),
													Child, KeyLabel1(GetCatStr( 81, "Script:"),'s'), Child, cy_Script = KeyCycle(CY_Protect,'s'),
													Child, KeyLabel1(GetCatStr( 82, "Pure:"),'p'), Child, cy_Pure = KeyCycle(CY_Protect,'p'),
													Child, KeyLabel1(GetCatStr( 83, "Archive:"),'a'), Child, cy_Archive = KeyCycle(CY_Protect,'a'),
													End,
												Child, ColGroup(2),
													Child, KeyLabel1(GetCatStr( 84,"Read:"),'r'), Child, cy_Read = KeyCycle(CY_Protect,'r'),
													Child, KeyLabel1(GetCatStr( 85,"Write:"),'w'), Child, cy_Write = KeyCycle(CY_Protect,'w'),
													Child, KeyLabel1(GetCatStr( 86,"Execute:"),'e'), Child, cy_Execute = KeyCycle(CY_Protect,'e'),
													Child, KeyLabel1(GetCatStr( 87,"Delete:"),'d'), Child, cy_Delete = KeyCycle(CY_Protect,'d'),
													End,
												End,
											Child, HGroup, Child, HSpace(0), End,
											Child, HGroup,
												MUIA_Group_SameSize, TRUE,
												Child, bt_Okay = SimpleButton(GetCatStr( 72, "_Okay" ) ),
												Child, HSpace(0),
												Child, bt_All = SimpleButton(GetCatStr( 88, "A_ll" ) ),
												Child, HSpace(0),
												Child, bt_Cancel = SimpleButton(GetCatStr( 73, "_Cancel" ) ),
												End,
											End,
										End;

									if ( !wi_Protection )
										Fail();

									DoMethod( wi_Protection, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2 );
									DoMethod( bt_Okay, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 1 );
									DoMethod( bt_Cancel, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2 );
									DoMethod( bt_All, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 3 );
									DoMethod( app_RumorOpus, OM_ADDMEMBER, wi_Protection );

									set( wi_Protection, MUIA_Window_Open, TRUE );
									set( wi_Protection, MUIA_Window_ActiveObject, bt_Okay);

									success = TRUE;
									while ( success )
									{
										switch ( DoMethod( app_RumorOpus, MUIM_Application_Input, &signal ) )
										{
											case MUIV_Application_ReturnID_Quit:
												global_QuitProgram = QuitRequester();
												if ( global_QuitProgram )
												{
													Cancel = TRUE;
													success = FALSE;
												}
												break;

											case 1:
												success = FALSE;
												break;

											case 2:
												Cancel = TRUE;
												success = FALSE;
												break;

											case 3:
												success = FALSE;
												All_BOOL = TRUE;
												break;
										}
										if ( signal ) Wait( signal );
									}

									set( wi_Protection, MUIA_Window_Open, FALSE );

									get( cy_Hide,    MUIA_Cycle_Active, &h );
									get( cy_Script,  MUIA_Cycle_Active, &s );
									get( cy_Pure,    MUIA_Cycle_Active, &p );
									get( cy_Archive, MUIA_Cycle_Active, &a );
									get( cy_Read,    MUIA_Cycle_Active, &r );
									get( cy_Write,   MUIA_Cycle_Active, &w );
									get( cy_Execute, MUIA_Cycle_Active, &e );
									get( cy_Delete,  MUIA_Cycle_Active, &d );

									DoMethod( app_RumorOpus, OM_REMMEMBER, wi_Protection );
									MUI_DisposeObject( wi_Protection );
								}

								if ( !Cancel )
								{
									Set_LONG = 0;
									fib = malloc( sizeof( struct FileInfoBlock ) );
									if ( fib )
									{
										lock = Lock( FileName_String, ACCESS_READ );
										if ( lock )
										{
											Examine( lock, fib );
											for( l = 0; l < 8; l++ ) ProtBits[7-l] = ( fib -> fib_Protection^0x0f ) & ( 1 << l ) ? "dewrapsh"[l] : '-'; ProtBits[8]=0;

											if ( ProtBits[0] == 'h' ) Set_LONG = Set_LONG + 128;
											if ( ProtBits[1] == 's' ) Set_LONG = Set_LONG + 64;
											if ( ProtBits[2] == 'p' ) Set_LONG = Set_LONG + 32;
											if ( ProtBits[3] == 'a' ) Set_LONG = Set_LONG + 16;
											if ( ProtBits[4] == '-' ) Set_LONG = Set_LONG + 8;
											if ( ProtBits[5] == '-' ) Set_LONG = Set_LONG + 4;
											if ( ProtBits[6] == '-' ) Set_LONG = Set_LONG + 2;
											if ( ProtBits[7] == '-' ) Set_LONG = Set_LONG + 1;

											if ( ( h == 1 ) && ( ProtBits[0] == '-' ) )	Set_LONG = Set_LONG + 128;
											if ( ( s == 1 ) && ( ProtBits[1] == '-' ) )	Set_LONG = Set_LONG + 64;
											if ( ( p == 1 ) && ( ProtBits[2] == '-' ) )	Set_LONG = Set_LONG + 32;
											if ( ( a == 1 ) && ( ProtBits[3] == '-' ) )	Set_LONG = Set_LONG + 16;
											if ( ( r == 1 ) && ( ProtBits[4] != 'r' ) )	Set_LONG = Set_LONG - 8;
											if ( ( w == 1 ) && ( ProtBits[5] != 'w' ) )	Set_LONG = Set_LONG - 4;
											if ( ( e == 1 ) && ( ProtBits[6] != 'e' ) )	Set_LONG = Set_LONG - 2;
											if ( ( d == 1 ) && ( ProtBits[7] != 'd' ) )	Set_LONG = Set_LONG - 1;

											if ( ( h == 2 ) && ( ProtBits[0] == 'h' ) )	Set_LONG = Set_LONG - 128;
											if ( ( s == 2 ) && ( ProtBits[1] == 's' ) )	Set_LONG = Set_LONG - 64;
											if ( ( p == 2 ) && ( ProtBits[2] == 'p' ) )	Set_LONG = Set_LONG - 32;
											if ( ( a == 2 ) && ( ProtBits[3] == 'a' ) )	Set_LONG = Set_LONG - 16;
											if ( ( r == 2 ) && ( ProtBits[4] != '-' ) )	Set_LONG = Set_LONG + 8;
											if ( ( w == 2 ) && ( ProtBits[5] != '-' ) )	Set_LONG = Set_LONG + 4;
											if ( ( e == 2 ) && ( ProtBits[6] != '-' ) )	Set_LONG = Set_LONG + 2;
											if ( ( d == 2 ) && ( ProtBits[7] != '-' ) )	Set_LONG = Set_LONG + 1;

											success = SetProtection( FileName_String, Set_LONG );
											if ( success )
												ReloadSrc_BOOL = TRUE;
											else
												ErrorNum = IoErr();
											UnLock( lock );
										}
										else
											ErrorNum = IoErr();
										free( fib );
									}
									else
										ErrorNum = -1;
								}
								break;
						}

						if ( Cancel || ErrorNum != 0 ) break;
						Action_BOOL = TRUE;

						if ( !NoDeselect_BOOL )
							DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Toggle, NULL );
						/* UpdateNumFiles(side); */

						k++;
						set( ga_Gauge, MUIA_Gauge_Current, k );
					}
					get( ga_Gauge, MUIA_Gauge_Max, &max );
					if ( max == k ) break;
				}

				set( wi_Progress, MUIA_Window_Open, FALSE );

				if ( Action_BOOL )
				{
					if ( ReloadSrc_BOOL )
						Reload( side );
					else
					{
						if ( cfg_Scroll && Jumped )
						{
							set( lv_Directory[side], MUIA_List_Quiet, TRUE );
							DoMethod( lv_Directory[side], MUIM_List_Jump, Entries_ULONG - 1 );
							Active_LONG = Active_LONG - ( Visible_LONG / 2 );
							if ( Active_LONG < 0 ) Active_LONG = 0;
							DoMethod( lv_Directory[side], MUIM_List_Jump, Active_LONG );
						}
						UpdateNumFiles( side );
					}

					if ( ReloadDst_BOOL && global_DirLoaded[OtherSide(side)] )
						Reload( OtherSide ( side ) );
				}

				if ( cfg_Scroll )
					set( lv_Directory[side], MUIA_List_Quiet, FALSE );
			}

			if ( !Action_BOOL && ( Operation == touch_entries ) )
			{
				NewName_String = StringRequester( GetCatStr( 89, "Enter file name" ), "", ":/", 31, 0, &Skip, &Cancel );
				if ( !Cancel && strlen( NewName_String ) > 0 )
				{
					sprintf( Target_String, "%s%s", GetPath( side ), NewName_String );
					if ( Exists( Target_String ) )
						if ( Overwrite( FileName_String, Target_String ) == 1 )
							ErrorNum = Delete( Target_String );
					if ( ErrorNum == 0 )
					{
						fh = Open( Target_String, MODE_NEWFILE );
						if ( fh )
						{
							Close( fh );
							if ( cfg_CreateIcons && ( ( strlen( FilePart( Target_String ) ) + 5 ) < 31 ) )
							{
								sprintf( String, "%s.info", Target_String );
								ErrorNum = CopyFile( "ENV:sys/def_project.info", String, FALSE );
							}
							Reload( side );
						}
						else
							ErrorNum = IoErr();
					}
				}
			}

			get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
			if ( Iconified )
			{
				switch ( cfg_Completed )
				{
					case 1:
						DisplayBeep( 0 );
						break;

					case 2:
						set( app_RumorOpus, MUIA_Application_Iconified, 0 );
						break;
				}
			}

			cfg_Overwrite = Over;
			cfg_DelForce = DelForce;
		}
	}
	global_AskOnce = FALSE;
	return( ErrorNum );
}
