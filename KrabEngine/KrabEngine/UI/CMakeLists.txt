target_sources(${ENGINE_TARGET_NAME}
    PUBLIC
        Canvas/WidgetCanvas.cpp
        Canvas/WidgetCanvas.hpp

        Components/Buttons/ButtonWidget.cpp
        Components/Buttons/ButtonWidget.hpp
        Components/Buttons/SpriteButton/SpriteButtonWidget.cpp
        Components/Buttons/SpriteButton/SpriteButtonWidget.hpp
        Components/Buttons/TextButton/TextButtonWidget.cpp
        Components/Buttons/TextButton/TextButtonWidget.hpp
        Components/ProgressBar/ProgressBarWidget.cpp
        Components/ProgressBar/ProgressBarWidget.hpp
        Components/Sprite/SpriteWidget.cpp
        Components/Sprite/SpriteWidget.hpp
        Components/Text/TextWidget.cpp
        Components/Text/TextWidget.hpp

        Layout/Grid/WidgetGrid.cpp
        Layout/Grid/WidgetGrid.hpp

        Layout/HorizontalBox/WidgetHorizontalBox.cpp
        Layout/HorizontalBox/WidgetHorizontalBox.hpp

        Layout/Plan/WidgetPlan.cpp
        Layout/Plan/WidgetPlan.hpp

        Layout/VerticalBox/WidgetVerticalBox.hpp
        Layout/VerticalBox/WidgetVerticalBox.cpp

        Widget/Widget.cpp
        Widget/Widget.hpp
)