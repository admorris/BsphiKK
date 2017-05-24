SetOptions[ $Output, PageWidth -> Infinity]
SetOptions[ $Output, FormatType -> OutputForm ]
$Assumptions = \
    Subscript[m,L] > 0 && Element[Subscript[m,L],Reals] \
 && Subscript[m,H] > 0 && Element[Subscript[m,H],Reals] \
 && Element[Subscript[\[CapitalDelta],m],Reals] \
 && Subscript[\[CapitalGamma],L] > 0 && Element[Subscript[\[CapitalGamma],L],Reals] \
 && Subscript[\[CapitalGamma],H] > 0 && Element[Subscript[\[CapitalGamma],L],Reals] \
 && Abs[Subscript[\[CapitalDelta],\[CapitalGamma]]] < 2 Subscript[\[CapitalGamma],tot] \
 && Element[Subscript[\[CapitalDelta],\[CapitalGamma]],Reals] \
 && Subscript[\[CapitalGamma],tot] > 0 && Element[Subscript[\[CapitalGamma],tot],Reals] \
 && Element[\[Lambda],Complexes] \
 && Element[t,Reals] && t > 0 \
 && Element[b,Reals] && b > 0 \
 && Element[n,Reals] && n > 0 \
 && Element[Subscript[t,0],Reals] && Subscript[t,0] > 0
(*Print[TeXForm[$Assumptions]]*)
(*OverBar[A][0] = Conjugate[A[0]]*)
(*
g[t_,sign_]:= (Exp[-(I Subscript[m,L] + Subscript[\[CapitalGamma],L]/2) t] + sign Exp[-(I Subscript[m,H] + Subscript[\[CapitalGamma],H]/2) t])/2
Print["g_{+}(t) &=&",TeXForm[g[t,+1]]]
Print["g_{-}(t) &=&",TeXForm[g[t,-1]]]
A[t_]          = A[0] g[t,+1] +     \[Lambda] OverBar[A][0] g[t,-1]
OverBar[A][t_] = OverBar[A][0] g[t,+1] + 1 / \[Lambda] A[0] g[t,-1]
Print[       "A_{h}(t) &=&",TeXForm[A[t]]]
Print["\\bar{A}_{h}(t) &=&",TeXForm[OverBar[A][t]]]
Aintegral   =Integrate[         Abs[A[t]]^2,{t,0,\[Infinity]}]
Abarintegral=Integrate[Abs[OverBar[A][t]]^2,{t,0,\[Infinity]}]
Print["\\int^{\\infty}_{0}        |A_{h}|^{2}(t) dt &=&",TeXForm[Aintegral   ]]
Print["\\int^{\\infty}_{0} |\\bar{A}_{h}|^{2}(t) dt &=&",TeXForm[Abarintegral]]
*)

Subscript[\[CapitalGamma],tot] := (Subscript[\[CapitalGamma],H]+Subscript[\[CapitalGamma],L])/2
Subscript[\[CapitalDelta],\[CapitalGamma]] := Subscript[\[CapitalGamma],H] - Subscript[\[CapitalGamma],L]
Subscript[\[CapitalDelta],m] := Subscript[m,H] - Subscript[m,L]

DecayRate[t_] = Exp[- Subscript[\[CapitalGamma],tot] t] (
  (Abs[A[0]]^2 + Abs[OverBar[A][0]]^2)/2 Cosh[Subscript[\[CapitalDelta],\[CapitalGamma]] t / 2]
+ (Abs[A[0]]^2 - Abs[OverBar[A][0]]^2)/2 Cos[Subscript[\[CapitalDelta],m] t]
+ (
    Re[ Conjugate[A[0]] OverBar[A][0] ] Sinh[Subscript[\[CapitalDelta],\[CapitalGamma]] t / 2]
  - Im[ Conjugate[A[0]] OverBar[A][0] ]  Sin[Subscript[\[CapitalDelta],m] t]
  )
)

DecayRateBar[t_] = Exp[- Subscript[\[CapitalGamma],tot] t] (
  (Abs[A[0]]^2 + Abs[OverBar[A][0]]^2)/2 Cosh[Subscript[\[CapitalDelta],\[CapitalGamma]] t / 2]
- (Abs[A[0]]^2 - Abs[OverBar[A][0]]^2)/2 Cos[Subscript[\[CapitalDelta],m] t]
+ (
    Re[ Conjugate[A[0]] OverBar[A][0] ] Sinh[Subscript[\[CapitalDelta],\[CapitalGamma]] t / 2]
  + Im[ Conjugate[A[0]] OverBar[A][0] ]  Sin[Subscript[\[CapitalDelta],m] t]
  )
)

Print[       "\\Gamma(t) &=&",TeXForm[DecayRate[t]]]
Print["\\\\"]
Print["\\bar{\\Gamma}(t) &=&",TeXForm[DecayRateBar[t]]]
Print["\\\\"]

tlow := Subscript[t,0]
timeintegral = Integrate[DecayRate[t] + DecayRateBar[t],{t,tlow,\[Infinity]}]
Print["\\int^{\\infty}_{",tlow,"} \\Gamma(t) + \\bar{\\Gamma}(t) dt &=&",TeXForm[Refine[timeintegral]]]
Print["\\\\"]

tlow := 0
timeintegral = Integrate[DecayRate[t] + DecayRateBar[t],{t,tlow,\[Infinity]}]
Print["\\int^{\\infty}_{",tlow,"} \\Gamma(t) + \\bar{\\Gamma}(t) dt &=&",TeXForm[Refine[timeintegral]]]
Print["\\\\"]

Acceptance[t_] = (((b (t - Subscript[t,0]))^n) / (1 + (b (t - Subscript[t,0]))^n)) HeavisideTheta[t - Subscript[t,0]]
timeintegral = Integrate[(DecayRate[t] + DecayRateBar[t]) Acceptance[t],{t,tlow,\[Infinity]}]
Print["\\varepsilon(t) &=&",Acceptance[t]]
Print["\\int^{\\infty}_{",tlow,"} \\left(\\Gamma(t) + \\bar{\\Gamma}(t)\\right) \\varepsilon(t) dt &=&",TeXForm[Refine[timeintegral]]]
Print["\\\\"]

Exit[]
