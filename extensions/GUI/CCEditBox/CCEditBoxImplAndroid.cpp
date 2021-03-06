/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 James Chen
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CCEditBoxImplAndroid.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "CCEditBox.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"


NS_CC_EXT_BEGIN

EditBoxImpl* __createSystemEditBox(EditBox* pEditBox)
{
    return new EditBoxImplAndroid(pEditBox);
}

EditBoxImplAndroid::EditBoxImplAndroid(EditBox* pEditText)
: EditBoxImpl(pEditText)
, _label(NULL)
, _labelPlaceHolder(NULL)
, _editBoxInputMode(kEditBoxInputModeSingleLine)
, _editBoxInputFlag(kEditBoxInputFlagInitialCapsAllCharacters)
, _keyboardReturnType(kKeyboardReturnTypeDefault)
, _colText(Color3B::WHITE)
, _colPlaceHolder(Color3B::GRAY)
, _maxLength(-1)
{
    
}

EditBoxImplAndroid::~EditBoxImplAndroid()
{
	
}

void EditBoxImplAndroid::doAnimationWhenKeyboardMove(float duration, float distance)
{ // don't need to be implemented on android platform.
	
}

static const int CC_EDIT_BOX_PADDING = 5;

bool EditBoxImplAndroid::initWithSize(const Size& size)
{
    int fontSize = getFontSizeAccordingHeightJni(size.height-12);
    _label = LabelTTF::create("", "", size.height-12);
	// align the text vertically center
    _label->setAnchorPoint(Point(0, 0.5f));
    _label->setPosition(Point(CC_EDIT_BOX_PADDING, size.height / 2.0f));
    _label->setColor(_colText);
    _editBox->addChild(_label);
	
    _labelPlaceHolder = LabelTTF::create("", "", size.height-12);
	// align the text vertically center
    _labelPlaceHolder->setAnchorPoint(Point(0, 0.5f));
    _labelPlaceHolder->setPosition(Point(CC_EDIT_BOX_PADDING, size.height / 2.0f));
    _labelPlaceHolder->setVisible(false);
    _labelPlaceHolder->setColor(_colPlaceHolder);
    _editBox->addChild(_labelPlaceHolder);
    
    _editSize = size;
    return true;
}

void EditBoxImplAndroid::setFont(const char* pFontName, int fontSize)
{
	if(_label != NULL) {
		_label->setFontName(pFontName);
		_label->setFontSize(fontSize);
	}
	
	if(_labelPlaceHolder != NULL) {
		_labelPlaceHolder->setFontName(pFontName);
		_labelPlaceHolder->setFontSize(fontSize);
	}
}

void EditBoxImplAndroid::setFontColor(const Color3B& color)
{
    _colText = color;
    _label->setColor(color);
}

void EditBoxImplAndroid::setPlaceholderFont(const char* pFontName, int fontSize)
{
	if(_labelPlaceHolder != NULL) {
		_labelPlaceHolder->setFontName(pFontName);
		_labelPlaceHolder->setFontSize(fontSize);
	}
}

void EditBoxImplAndroid::setPlaceholderFontColor(const Color3B& color)
{
    _colPlaceHolder = color;
    _labelPlaceHolder->setColor(color);
}

void EditBoxImplAndroid::setInputMode(EditBoxInputMode inputMode)
{
    _editBoxInputMode = inputMode;
}

void EditBoxImplAndroid::setMaxLength(int maxLength)
{
    _maxLength = maxLength;
}

int EditBoxImplAndroid::getMaxLength()
{
    return _maxLength;
}

void EditBoxImplAndroid::setInputFlag(EditBoxInputFlag inputFlag)
{
    _editBoxInputFlag = inputFlag;
}

void EditBoxImplAndroid::setReturnType(KeyboardReturnType returnType)
{
    _keyboardReturnType = returnType;
}

bool EditBoxImplAndroid::isEditing()
{
    return false;
}

void EditBoxImplAndroid::setText(const char* pText)
{
    if (pText != NULL)
    {
        _text = pText;
		
        if (_text.length() > 0)
        {
            _labelPlaceHolder->setVisible(false);
			
            std::string strToShow;
			
            if (kEditBoxInputFlagPassword == _editBoxInputFlag)
            {
                long length = cc_utf8_strlen(_text.c_str(), -1);
                for (long i = 0; i < length; i++)
                {
                    strToShow.append("\u25CF");
                }
            }
            else
            {
                strToShow = _text;
            }

			_label->setString(strToShow.c_str());

			// Clip the text width to fit to the text box
			float fMaxWidth = _editSize.width - CC_EDIT_BOX_PADDING * 2;
			Rect clippingRect = _label->getTextureRect();
			if(clippingRect.size.width > fMaxWidth) {
				clippingRect.size.width = fMaxWidth;
				_label->setTextureRect(clippingRect);
			}

        }
        else
        {
            _labelPlaceHolder->setVisible(true);
            _label->setString("");
        }
		
    }
}

const char*  EditBoxImplAndroid::getText(void)
{
    return _text.c_str();
}

void EditBoxImplAndroid::setPlaceHolder(const char* pText)
{
    if (pText != NULL)
    {
        _placeHolder = pText;
        if (_placeHolder.length() > 0 && _text.length() == 0)
        {
            _labelPlaceHolder->setVisible(true);
        }
		
        _labelPlaceHolder->setString(_placeHolder.c_str());
    }
}

void EditBoxImplAndroid::setPosition(const Point& pos)
{ // don't need to be implemented on android platform.
	
}

void EditBoxImplAndroid::setVisible(bool visible)
{ // don't need to be implemented on android platform.

}

void EditBoxImplAndroid::setContentSize(const Size& size)
{ // don't need to be implemented on android platform.
	
}

void EditBoxImplAndroid::setAnchorPoint(const Point& anchorPoint)
{ // don't need to be implemented on android platform.
	
}

void EditBoxImplAndroid::visit(void)
{ // don't need to be implemented on android platform.
    
}

void EditBoxImplAndroid::onEnter(void)
{ // don't need to be implemented on android platform.
    
}

static void editBoxCallbackFunc(const char* pText, void* ctx)
{
    EditBoxImplAndroid* thiz = (EditBoxImplAndroid*)ctx;
    thiz->setText(pText);
	
    if (thiz->getDelegate() != NULL)
    {
        thiz->getDelegate()->editBoxTextChanged(thiz->getEditBox(), thiz->getText());
        thiz->getDelegate()->editBoxEditingDidEnd(thiz->getEditBox());
        thiz->getDelegate()->editBoxReturn(thiz->getEditBox());
    }
    
    EditBox* pEditBox = thiz->getEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {        
        CommonScriptData data(pEditBox->getScriptEditBoxHandler(), "changed",pEditBox);
        ScriptEvent event(kCommonEvent,(void*)&data);
        ScriptEngineManager::sharedManager()->getScriptEngine()->sendEvent(&event);
        memset(data.eventName,0,64*sizeof(char));
        strncpy(data.eventName,"ended",64);
        event.data = (void*)&data;
        ScriptEngineManager::sharedManager()->getScriptEngine()->sendEvent(&event);
        memset(data.eventName,0,64*sizeof(char));
        strncpy(data.eventName,"return",64);
        event.data = (void*)&data;
        ScriptEngineManager::sharedManager()->getScriptEngine()->sendEvent(&event);
    }
}

void EditBoxImplAndroid::openKeyboard()
{
    if (_delegate != NULL)
    {
        _delegate->editBoxEditingDidBegin(_editBox);
    }
    EditBox* pEditBox = this->getEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {        
        CommonScriptData data(pEditBox->getScriptEditBoxHandler(), "began",pEditBox);
        ScriptEvent event(cocos2d::kCommonEvent,(void*)&data);
        ScriptEngineManager::sharedManager()->getScriptEngine()->sendEvent(&event);
    }
	
    showEditTextDialogJNI(  _placeHolder.c_str(),
						  _text.c_str(),
						  _editBoxInputMode,
						  _editBoxInputFlag,
						  _keyboardReturnType,
						  _maxLength,
						  editBoxCallbackFunc,
						  (void*)this  );
	
}

void EditBoxImplAndroid::closeKeyboard()
{
	
}

NS_CC_EXT_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) */

