#include "Global.h"

#include "Fonts.hpp"
#include "MsgBox.hpp"
#include "DialogType.hpp"
#include "GameEvents.hpp"
#include "UserInterface.hpp"

#include "..\RGE\Image.hpp"
#include "..\RGE\Button.hpp"
#include "..\RGE\IRender.hpp"
#include "..\RGE\ListBox.hpp"
#include "..\RGE\UiState.hpp"

MsgBox::MsgBox()
	: mButtonOK(0), mButtonCancel(0), mListBox(0),
	  mImageTop(0), mImageMiddle(0), mImageBottom(0)
{
	SetID(DLG_MSGBOX);
}

MsgBox::~MsgBox(){}

MsgBox* MsgBox::Create(){
	MsgBox* dlg = new MsgBox();
	dlg->CreateFromXML("3DDATA\\CONTROL\\XML\\MsgBox.xml");
	return dlg;
}

void MsgBox::OnCreated(){
	mImageTop = (Image*)GetControlByID(5);
	mImageMiddle = (Image*)GetControlByID(6);
	mImageBottom = (Image*)GetControlByID(7);

	mListBox = (ListBox*)GetControlByID(10);

	mButtonOK = (Button*)GetControlByID(255);
	mButtonCancel = (Button*)GetControlByID(256);
	
	mListBox->AddItem("Message");

	mImageTop->SetPosition(Point(0, 0));
	mImageMiddle->SetPosition(Point(0, 0));
	mImageBottom->SetPosition(Point(0, 0));

	mButtonOK->SetPosition(Point(0, 0));
	mButtonCancel->SetPosition(Point(0, 0));

	mListBox->SetPositionX(15);
}

int MsgBox::HandleEvent(MouseEvent* evt){
	int id = Dialog::HandleEvent(evt);
	if(id == mButtonOK->ID() || id == mButtonCancel->ID()){
		MsgBoxEvent* evt = gGameEvents->CreateEvent<MsgBoxEvent>();
		evt->mStatus = (id == mButtonOK->ID())?BUTTON_OK:BUTTON_CANCEL;
		gGameEvents->Emit(evt);
		SetVisible(false);
		UiState::mModal = NULL;
	}

	return id;
}

void MsgBox::Render(){
	if(!Visible()) return;

	gUiRender->PushTranslate(mPosition.x, mPosition.y);
	
	mImageTop->Render();

	int y = mImageTop->SizeY();
	int yInc = mImageMiddle->SizeY();
	for(unsigned int i = 0; i < mListBox->ItemCount(); ++i, y += yInc){
		mImageMiddle->SetPositionY(y);
		mImageMiddle->Render();
	}

	mListBox->Render();

	mImageBottom->Render();
	mButtonOK->Render();
	mButtonCancel->Render();

	gUiRender->PopTranslate(mPosition.x, mPosition.y);
}

void MsgBox::SetButtons(int buttons){
	if(buttons == 0){
		mButtonOK->SetVisible(false);
		mButtonCancel->SetVisible(false);
	}else if(buttons == BUTTON_OK){
		mButtonCancel->SetVisible(false);
		mButtonOK->SetVisible(true);

		mButtonOK->SetPositionX((mImageBottom->SizeX() / 2) - (mButtonOK->SizeX() / 2));
	}else if(buttons == BUTTON_CANCEL){
		mButtonOK->SetVisible(false);
		mButtonCancel->SetVisible(true);

		mButtonCancel->SetPositionX((mImageBottom->SizeX() / 2) - (mButtonOK->SizeX() / 2));
	}else if(buttons == (BUTTON_CANCEL | BUTTON_OK)){
		mButtonOK->SetVisible(true);
		mButtonCancel->SetVisible(true);

		int div4 = mImageBottom->SizeX() / 4;
		mButtonOK->SetPositionX(div4 - (mButtonOK->SizeX() / 2));
		mButtonCancel->SetPositionX((div4 * 3) - (mButtonCancel->SizeX() / 2));
	}
}

void MsgBox::SetMessage(const char* message){
	try{
		if(mListBox->GetItem(1))
			mListBox->DeleteItem(1);
		int messageSize = strlen(message);
		if(messageSize >= 21)
		{
			char *newstr; // our new string
			int i, x;
			for(i = 0, x = 0; i <= messageSize; i++)
			{
				if(message[i] == 0x20 &&  i >= 15 && x == 0) //try one to find a space after 15 chars
				{
					break;
				}
				else if(message[i] == 0x20 && i >= 10 && x == 1) // try 2 to find a space after 10 chars
				{
					break;
				}
				else if(message[i] == 0x20 && i >= 5 && x == 2) // try 3 to find a space after 5 chars
				{
					break;
				}
				else if(message[i] == 0x20 && x == 3) // if the last 3 didn't do it stop at any space
				{
					break;
				}
				else if(i == messageSize) // if one the first 3 if's failed
				{
					x++;
					i = 0;
				}
			}
			messageSize -= i; // make it the size of the last x chars
			newstr = new char[messageSize];
			memset(newstr, 0, messageSize);

			char *tempstr = new char[messageSize];
			memset(tempstr, 0, messageSize); // clear the string

			memmove(tempstr+0, message+i, messageSize); // move the end of the string in a new string
			tempstr[messageSize] = 0; // make our string zero terminated

			// strcpy_s doesn't work in this case, so disable the warnings, that's less work then manualy copying the string
			#pragma warning(push)
			#pragma warning(disable:4996)
			#pragma warning(disable:4995)
			strcpy(newstr, tempstr);
			#pragma warning(pop)
			memmove(tempstr+0, message+0, i);
			tempstr[i] = 0;

			message = tempstr;
			mListBox->SetItem(0, message);
			mListBox->AddItem(newstr);
		}
		else
		{
			mListBox->SetItem(0, message);
		}
	}
	catch(std::exception& e)
	{
		OutputDebugString(e.what());
		LOG("Exception occured: %s", e.what());
	}
	catch(...)
	{
		OutputDebugString("Error!");
		LOG("Error! 0X%x", GetLastError());
	}
	mListBox->SetItem(0, message);

	int bottom = mImageTop->SizeY() + (mImageMiddle->SizeY() * mListBox->ItemCount());
	mImageBottom->SetPositionY(bottom);

	int halfBtm = mImageBottom->SizeY() / 2;
	bottom += halfBtm;
	mButtonOK->SetPositionY(bottom - (mButtonOK->SizeY() / 2));
	mButtonCancel->SetPositionY(bottom - (mButtonCancel->SizeY() / 2));

	bottom += halfBtm;
	SetSizeY(bottom);
}