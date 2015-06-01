
#include <TurboBadger/tb_widgets.h>
#include <TurboBadger/tb_widgets_common.h>

#include <Atomic/IO/Log.h>

#include "UIEvents.h"
#include "UI.h"
#include "UIButton.h"

using namespace tb;

namespace Atomic
{

UIButton::UIButton(Context* context, bool createWidget) : UIWidget(context, false)
{
    if (createWidget)
    {
        widget_ = new TBButton();
        widget_->SetDelegate(this);
        GetSubsystem<UI>()->WrapWidget(this, widget_);
    }
}

UIButton::~UIButton()
{

}

void UIButton::SetSqueezable(bool value)
{
    if (!widget_)
        return;

    ((TBButton*)widget_)->SetSqueezable(value);
}

bool UIButton::OnEvent(const tb::TBWidgetEvent &ev)
{
    return UIWidget::OnEvent(ev);
}

}
