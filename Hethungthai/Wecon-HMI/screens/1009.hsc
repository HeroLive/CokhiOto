<?xml version="1.0" encoding="utf-8"?>
<ScrInfo ScreenNo="0" ScreenType="" ScreenSize="0">
	<Script>
		<TimerAction/>
		<TrigAction>
			<Trigger Action="1" BitAddr="HSX4077.0">'设置期数加1
@W_HSW4077 =0 
if @W_HSW4079&lt;@W_HSW4088 then   '当前设置的期数小于总期数
   @W_HSW4079=@W_HSW4079+1
   @W_HSW4073=1'用于触发当前期数信息给start Unit
endif
</Trigger>
			<Trigger Action="1" BitAddr="HSX4078.0">'设置期数减1
@W_HSW4078 =0 
if @W_HSW4079&gt;1 then   '当前设置的期数&gt;0
   @W_HSW4079=@W_HSW4079-1
   @W_HSW4073=1 '用于触发当前期数信息给start Unit
endif



</Trigger>
		</TrigAction>
		<InitialAction>@W_HSW4079=1
@W_HSW4073=1

</InitialAction>
	</Script>
<PartInfo PartType="WordSwitch" PartName="WS_6">
<General Desc="WS_1" Area="22 220 114 256" WordAddr="HSW4075" WriteAddr="HSW4075" DataFormat="2" Const="1" Limit="12" FigureFile="" BorderColor="0xcccccc 16777215" BmpIndex="-1" LaStartPt="65 18" IsIndirectR="0" IsIndirectW="0" IsWordOrder="0"/>
<Extension IsCheck="0" AckTime="20" UseShowHide="0" HideType="0" IsHideAllTime="0" Lockmate="0" DrawLock="0" IsShowGrayScale="0" LockMode="0" TouchState="1" Buzzer="1" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/>
<Label Status="0" Pattern="1" FrnColor="0xffffff 0" BgColor="0xffffff 0" Bold="0" LaIndexID="" CharSize="8 168 168 168 168 168 168 168 16" LaFrnColor="0x0 0"/></PartInfo>
<PartInfo PartType="WordSwitch" PartName="WS_0">
<General Desc="WS_0" Area="22 221 114 257" WordAddr="HSW4078" WriteAddr="HSW4078" DataFormat="2" Const="1" FigureFile="TFT-type style\TFT001.pvg" BorderColor="0xffffff 16777215" BmpIndex="-1" LaStartPt="0 10" IsIndirectR="0" IsIndirectW="0" IsWordOrder="0"/>
<Extension IsCheck="0" UseShowHide="0" HideType="0" IsHideAllTime="0" Lockmate="0" DrawLock="0" IsShowGrayScale="0" LockMode="0" TouchState="1" Buzzer="1" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/>
<Label Status="0" Pattern="-894245163" FrnColor="0xffffff 0" BgColor="0xffffff 0" Bold="0" LaIndexID="Prev periodPrev periodPrev periodPrev periodPrev periodPrev periodPrev periodPrev period" CharSize="8 168 168 168 168 168 168 168 16" LaFrnColor="0x0 0"/></PartInfo>
<PartInfo PartType="WordSwitch" PartName="WS_5">
<General Desc="WS_1" Area="119 220 211 256" WordAddr="HSW4075" WriteAddr="HSW4075" DataFormat="2" Const="1" Limit="12" FigureFile="" BorderColor="0xcccccc 16777215" BmpIndex="-1" LaStartPt="34 16" IsIndirectR="0" IsIndirectW="0" IsWordOrder="0"/>
<Extension IsCheck="0" AckTime="20" UseShowHide="0" HideType="0" IsHideAllTime="0" Lockmate="0" DrawLock="0" IsShowGrayScale="0" LockMode="0" TouchState="1" Buzzer="1" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/>
<Label Status="0" Pattern="1" FrnColor="0xffffff 0" BgColor="0xffffff 0" Bold="0" LaIndexID="" CharSize="8 168 168 168 168 168 168 168 16" LaFrnColor="0x0 0"/></PartInfo>
<PartInfo PartType="WordSwitch" PartName="WS_1">
<General Desc="WS_1" Area="118 221 210 257" WordAddr="HSW4077" WriteAddr="HSW4077" DataFormat="2" Const="1" Limit="12" FigureFile="TFT-type style\TFT001.pvg" BorderColor="0xffffff 16777215" BmpIndex="-1" LaStartPt="0 10" IsIndirectR="0" IsIndirectW="0" IsWordOrder="0"/>
<Extension IsCheck="0" AckTime="20" UseShowHide="0" HideType="0" IsHideAllTime="0" Lockmate="0" DrawLock="0" IsShowGrayScale="0" LockMode="0" TouchState="1" Buzzer="1" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/>
<Label Status="0" Pattern="1" FrnColor="0xffffff 0" BgColor="0xffffff 0" Bold="0" LaIndexID="Next periodNext periodNext periodNext periodNext periodNext periodNext periodNext period" CharSize="8 168 168 168 168 168 168 168 16" LaFrnColor="0x0 0"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_4">
<General TextContent="Total periods:Total periods:Total periods:Total periods:Total periods:Total periods:Total periods:Total periods:" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="34 20"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Numeric" PartName="NUM_1">
<General Desc="NUM_0" Area="177 13 238 44" WordAddr="HSW4088" Fast="0" IsInput="1" WriteAddr="HSW4088" KbdScreen="-6" IsPopKeyBrod="0" FigureFile="" BorderColor="0xcccccc 16777215" FrnColor="0x0 0" BgColor="0xffffff 0" BmpIndex="-1" Transparent="0" IsHideNum="0" HighZeroPad="0" IsShowPwd="0" UseGlint="0" GlintFgClr="0x0 0" ZeroNoDisplay="0" IsIndirectR="0" IsIndirectW="0" IsAddFrame="0" IsWordOrder="0"/>
<DispFormat DispType="2" DigitCount="4 0" DataLimit="0 1094713344" IsVar="0" Zoom="0" Mutiple="1.000000" Round="0" CharSize="8 16" IsInputLabelL="0" IsInputLabelR="0" IsInputDefault="0" bShowRange="0" IsVar1="0" ColorHText="0x0 0" ColorHBag="0x0 0" ColorLText="0x0 0" ColorLBag="0x0 0"/>
<Extension IsCheck="0" AckTime="20" Lockmate="0" DrawLock="0" LockMode="0" UseShowHide="0" HideType="0" IsHideAllTime="0" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_6">
<General TextContent="Password:Password:Password:Password:Password:Password:Password:Password:" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="50 136"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="String" PartName="STR_1">
<General Desc="STR_0" Area="140 129 308 160" WordAddr="HSW4090" Fast="0" stCount="8" IsInput="1" WriteAddr="HSW4090" KbdScreen="-1" IsPopKeyBrod="0" FigureFile="" BorderColor="0xcccccc 16777215" FrnColor="0x0 0" BgColor="0xffffff 0" CharSize="8 16" IsHideNum="0" Transparent="0" IsShowPwd="0" IsIndirectR="0" IsIndirectW="0" IsInputDefault="0" IsDWord="0" IsHiLowRever="0"/>
<Extension IsCheck="0" AckTime="5" Lockmate="0" DrawLock="0" LockMode="0" UseShowHide="0" HideType="0" IsHideAllTime="0" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_7">
<General TextContent="Current is:Current is:Current is:Current is:Current is:Current is:Current is:Current is:" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="34 89"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_8">
<General TextContent="periodperiodperiodperiodperiodperiodperiodperiod" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="245 88"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Numeric" PartName="NUM_2">
<General Desc="NUM_0" Area="140 174 192 205" WordAddr="HSW4094" Fast="0" IsInput="1" WriteAddr="HSW4094" KbdScreen="-6" IsPopKeyBrod="0" FigureFile="" BorderColor="0xcccccc 16777215" FrnColor="0xffffff -1" BgColor="0xff8000 -1" BmpIndex="-1" Transparent="0" IsHideNum="0" HighZeroPad="0" IsShowPwd="0" UseGlint="0" GlintFgClr="0x0 0" ZeroNoDisplay="0" IsIndirectR="0" IsIndirectW="0" IsAddFrame="0" IsWordOrder="0"/>
<DispFormat DispType="2" DigitCount="4 0" DataLimit="1156988928 1161527296" IsVar="0" Zoom="0" Mutiple="1.000000" Round="0" CharSize="8 16" IsInputLabelL="0" IsInputLabelR="0" IsInputDefault="0" bShowRange="0" IsVar1="0" ColorHText="0x0 0" ColorHBag="0x0 0" ColorLText="0x0 0" ColorLBag="0x0 0"/>
<Extension IsCheck="0" AckTime="20" Lockmate="0" DrawLock="0" LockMode="0" UseShowHide="0" HideType="0" IsHideAllTime="0" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Numeric" PartName="NUM_3">
<General Desc="NUM_0" Area="248 174 295 205" WordAddr="HSW4095" Fast="0" IsInput="1" WriteAddr="HSW4095" KbdScreen="-6" IsPopKeyBrod="0" FigureFile="" BorderColor="0xcccccc 16777215" FrnColor="0xffffff -1" BgColor="0xff8000 -1" BmpIndex="-1" Transparent="0" IsHideNum="0" HighZeroPad="0" IsShowPwd="0" UseGlint="0" GlintFgClr="0x0 0" ZeroNoDisplay="0" IsIndirectR="0" IsIndirectW="0" IsAddFrame="0" IsWordOrder="0"/>
<DispFormat DispType="2" DigitCount="2 0" DataLimit="1065353216 1094713344" IsVar="0" Zoom="0" Mutiple="1.000000" Round="0" CharSize="8 16" IsInputLabelL="0" IsInputLabelR="0" IsInputDefault="0" bShowRange="0" IsVar1="0" ColorHText="0x0 0" ColorHBag="0x0 0" ColorLText="0x0 0" ColorLBag="0x0 0"/>
<Extension IsCheck="0" AckTime="20" Lockmate="0" DrawLock="0" LockMode="0" UseShowHide="0" HideType="0" IsHideAllTime="0" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Numeric" PartName="NUM_4">
<General Desc="NUM_0" Area="350 174 384 205" WordAddr="HSW4096" Fast="0" IsInput="1" WriteAddr="HSW4096" KbdScreen="-6" IsPopKeyBrod="0" FigureFile="" BorderColor="0xcccccc 16777215" FrnColor="0xffffff -1" BgColor="0xff8000 -1" BmpIndex="-1" Transparent="0" IsHideNum="0" HighZeroPad="0" IsShowPwd="0" UseGlint="0" GlintFgClr="0x0 0" ZeroNoDisplay="0" IsIndirectR="0" IsIndirectW="0" IsAddFrame="0" IsWordOrder="0"/>
<DispFormat DispType="2" DigitCount="2 0" DataLimit="1065353216 1106771968" IsVar="0" Zoom="0" Mutiple="1.000000" Round="0" CharSize="8 16" IsInputLabelL="0" IsInputLabelR="0" IsInputDefault="0" bShowRange="0" IsVar1="0" ColorHText="0x0 0" ColorHBag="0x0 0" ColorLText="0x0 0" ColorLBag="0x0 0"/>
<Extension IsCheck="0" AckTime="20" Lockmate="0" DrawLock="0" LockMode="0" UseShowHide="0" HideType="0" IsHideAllTime="0" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_0">
<General TextContent="YearYearYearYearYearYearYearYear" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="195 180"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_1">
<General TextContent="MonthMonthMonthMonthMonthMonthMonthMonth" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="297 180"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_2">
<General TextContent="DayDayDayDayDayDayDayDay" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="387 180"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Rect" PartName="REC_1">
<General Area="35 117 442 213" LineType="10" BorderColor="0x0 -1" Pattern="-1" FrnColor="0xffffff 0" BgColor="0xffffff 0" ActiveColor="0"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_3">
<General TextContent="Due date:Due date:Due date:Due date:Due date:Due date:Due date:Due date:" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="51 181"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_9">
<General TextContent="Supper passwords:Supper passwords:Supper passwords:Supper passwords:Supper passwords:Supper passwords:Supper passwords:Supper passwords:" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="35 56"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="String" PartName="STR_0">
<General Desc="STR_0" Area="177 47 345 78" WordAddr="HSW4084" Fast="0" stCount="8" IsInput="1" WriteAddr="HSW4084" KbdScreen="-1" IsPopKeyBrod="0" FigureFile="" BorderColor="0xcccccc 16777215" FrnColor="0x0 0" BgColor="0xffffff 0" CharSize="8 16" IsHideNum="0" Transparent="0" IsShowPwd="0" IsIndirectR="0" IsIndirectW="0" IsInputDefault="0" IsDWord="0" IsHiLowRever="0"/>
<Extension IsCheck="0" AckTime="5" Lockmate="0" DrawLock="0" LockMode="0" UseShowHide="0" HideType="0" IsHideAllTime="0" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_5">
<General TextContent="" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="147 89"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Numeric" PartName="NUM_0">
<General Desc="NUM_0" Area="177 81 238 112" WordAddr="HSW4079" Fast="0" IsInput="0" KbdScreen="-6" IsPopKeyBrod="0" FigureFile="" BorderColor="0xcccccc 16777215" FrnColor="0xffffff -1" BgColor="0xff8000 -1" BmpIndex="-1" Transparent="0" IsHideNum="0" HighZeroPad="0" IsShowPwd="0" UseGlint="0" GlintFgClr="0x0 0" ZeroNoDisplay="0" IsIndirectR="0" IsIndirectW="0" IsAddFrame="0" IsWordOrder="0"/>
<DispFormat DispType="2" DigitCount="2 0" DataLimit="0 1120272384" IsVar="0" Zoom="0" Mutiple="1.000000" Round="0" CharSize="8 16" IsInputLabelL="0" IsInputLabelR="0" IsInputDefault="0" bShowRange="0" IsVar1="0" ColorHText="0x0 0" ColorHBag="0x0 0" ColorLText="0x0 0" ColorLBag="0x0 0"/>
<Extension IsCheck="0" AckTime="20" Lockmate="0" DrawLock="0" LockMode="0" UseShowHide="0" HideType="0" IsHideAllTime="0" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="WordSwitch" PartName="WS_2">
<General Desc="WS_1" Area="213 221 288 257" WordAddr="HSW4075" WriteAddr="HSW4075" DataFormat="2" Const="1" Limit="12" FigureFile="TFT-type style\TFT001.pvg" BorderColor="0xffffff 16777215" BmpIndex="-1" LaStartPt="34 16" IsIndirectR="0" IsIndirectW="0" IsWordOrder="0"/>
<Extension IsCheck="0" AckTime="20" UseShowHide="0" HideType="0" IsHideAllTime="0" Lockmate="0" DrawLock="0" IsShowGrayScale="0" LockMode="0" TouchState="1" Buzzer="1" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/>
<Label Status="0" Pattern="1" FrnColor="0xffffff 0" BgColor="0xffffff 0" Bold="0" LaIndexID="SaveSaveSaveSaveSaveSaveSaveSave" CharSize="8 168 168 168 168 168 168 168 16" LaFrnColor="0x0 0"/></PartInfo>
<PartInfo PartType="WordSwitch" PartName="WS_3">
<General Desc="WS_1" Area="291 221 366 257" WordAddr="HSW4076" WriteAddr="HSW4076" DataFormat="2" Const="1" Limit="12" FigureFile="TFT-type style\TFT001.pvg" BorderColor="0xffffff 16777215" BmpIndex="-1" LaStartPt="34 16" IsIndirectR="0" IsIndirectW="0" IsWordOrder="0"/>
<Extension IsCheck="0" AckTime="20" UseShowHide="0" HideType="0" IsHideAllTime="0" Lockmate="0" DrawLock="0" IsShowGrayScale="0" LockMode="0" TouchState="1" Buzzer="1" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/>
<Label Status="0" Pattern="1" FrnColor="0xffffff 0" BgColor="0xffffff 0" Bold="0" LaIndexID="CancelCancelCancelCancelCancelCancelCancelCancel" CharSize="8 168 168 168 168 168 168 168 16" LaFrnColor="0x0 0"/></PartInfo>
<PartInfo PartType="WordSwitch" PartName="WS_4">
<General Desc="WS_1" Area="368 221 443 257" WordAddr="HSW4074" WriteAddr="HSW4074" DataFormat="2" Const="1" Limit="12" FigureFile="TFT-type style\TFT001.pvg" BorderColor="0xffffff 16777215" BmpIndex="-1" LaStartPt="34 16" IsIndirectR="0" IsIndirectW="0" IsWordOrder="0"/>
<Extension IsCheck="0" AckTime="20" UseShowHide="0" HideType="0" IsHideAllTime="0" Lockmate="0" DrawLock="0" IsShowGrayScale="0" LockMode="0" TouchState="1" Buzzer="1" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/>
<Label Status="0" Pattern="1" FrnColor="0xffffff 0" BgColor="0xffffff 0" Bold="0" LaIndexID="ExitExitExitExitExitExitExitExit" CharSize="8 168 168 168 168 168 168 168 16" LaFrnColor="0x0 0"/></PartInfo>
<PartInfo PartType="Text" PartName="TXT_10">
<General TextContent="Start period:Start period:Start period:Start period:Start period:Start period:Start period:Start period:" LaFrnColor="0x0 0" IsBackColor="0" BgColor="0x0 0" CharSize="8 168 168 168 168 168 168 168 16" Bold="0" StartPt="242 20"/>
<MoveZoom DataFormatMZ="2"/></PartInfo>
<PartInfo PartType="Numeric" PartName="NUM_5">
<General Desc="NUM_0" Area="383 13 444 44" WordAddr="HSW4089" Fast="0" IsInput="1" WriteAddr="HSW4089" KbdScreen="-6" IsPopKeyBrod="0" FigureFile="" BorderColor="0xcccccc 16777215" FrnColor="0x0 0" BgColor="0xffffff 0" BmpIndex="-1" Transparent="0" IsHideNum="0" HighZeroPad="0" IsShowPwd="0" UseGlint="0" GlintFgClr="0x0 0" ZeroNoDisplay="0" IsIndirectR="0" IsIndirectW="0" IsAddFrame="0" IsWordOrder="0"/>
<DispFormat DispType="2" DigitCount="4 0" DataLimit="0 1094713344" IsVar="0" Zoom="0" Mutiple="1.000000" Round="0" CharSize="8 16" IsInputLabelL="0" IsInputLabelR="0" IsInputDefault="0" bShowRange="0" IsVar1="0" ColorHText="0x0 0" ColorHBag="0x0 0" ColorLText="0x0 0" ColorLBag="0x0 0"/>
<Extension IsCheck="0" AckTime="20" Lockmate="0" DrawLock="0" LockMode="0" UseShowHide="0" HideType="0" IsHideAllTime="0" IsUesPartPassword="0" IsSetLowerLev="0" IsUseUserAuthority="0"/>
<MoveZoom DataFormatMZ="2"/></PartInfo></ScrInfo>
