#ifndef _levelselectcell_h
#define _levelselectcell_h

#include <mezzanine.h>

using namespace Mezzanine;

class LevelSelectCell : public UI::Cell
{
    protected:
        UI::Rectangle* CellBack;
        UI::Rectangle* PreviewImage;
        UI::Rectangle* PreviewBorder;
        UI::Caption* LevelTitle;
        UI::Caption* EarnedMaxScore;
        Vector2 PreviewImageOffset;
        Vector2 PreviewBorderOffset;
        Vector2 LevelTitleOffset;
        Vector2 EarnedMaxScoreOffset;
        virtual void UpdateImpl(bool Force = false);
        virtual void SetVisibleImpl(bool visible);
        virtual bool CheckMouseHoverImpl();
        virtual void CalculateOffsets(const Vector2& Size);
    public:
        LevelSelectCell(const String& name, const UI::RenderableRect& Rect, UI::Layer* parent);
        virtual ~LevelSelectCell();
        virtual void SetPosition(const Vector2& Position);
        virtual void SetActualPosition(const Vector2& Position);
        virtual void SetSize(const Vector2& Size);
        virtual void SetActualSize(const Vector2& Size);
        virtual void DoSelectedItems();
        virtual void UpdateDimensions();
        virtual String GetLevelName();
        virtual UI::Rectangle* GetCellBack();
        virtual UI::Rectangle* GetPreviewImage();
        virtual UI::Rectangle* GetPreviewBorder();
        virtual UI::Caption* GetLevelTitle();
        virtual UI::Caption* GetEarnedMaxScore();
};

class LevelSelectCB : public UI::CellCallback
{
    protected:
        LevelSelectCell* LevelCell;
    public:
        LevelSelectCB();
        virtual ~LevelSelectCB();
        virtual void SetCaller(UI::Cell* Caller);
        virtual void DoSelectedItems();
        virtual void DoUnselectedItems();
};

#endif
