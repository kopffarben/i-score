#pragma once
#include <Curve/Process/CurveProcessFactory.hpp>
#include <Interpolation/InterpolationColors.hpp>
#include <Interpolation/InterpolationProcess.hpp>
#include <Interpolation/InterpolationLayer.hpp>
#include <Interpolation/InterpolationPresenter.hpp>
#include <Interpolation/InterpolationView.hpp>
#include <Process/GenericProcessFactory.hpp>

namespace Interpolation
{
using InterpolationFactory = Process::GenericProcessModelFactory<ProcessModel>;
using InterpolationLayerFactory =
Curve::CurveLayerFactory_T<
ProcessModel, Layer, Presenter, View, Colors>;
}

