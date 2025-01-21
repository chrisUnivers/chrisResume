import * as Plot from "npm:@observablehq/plot";
import {extent, format, rollup, timeFormat} from "npm:d3";

function friendlyTypeName(d) {
  switch (d) {
    case "demandActual":
      return "Demand (actual)";
    case "demandForecast":
      return "Demand (forecast)";
    case "netGeneration":
      return "Net generation";
  }
}

// US electricity demand, generation and forecasting chart
export function usGenDemandForecastChart(width, height, data, currentHour) {
  // Roll up each hour's values into a single row for a cohesive tip
  const compoundTips = rollup(
    data,
    (v) => ({...v[0], ...Object.fromEntries(v.map(({name, value}) => [name, value]))}),
    (d) => d.date
  ).values();

  return Plot.plot({
    width,
    marginTop: 0,
    height: height - 50,
    y: {tickSize: 0, label: null},
    x: {type: "time", tickSize: 0, tickPadding: 3, label: null},
    color: {
      legend: true,
      domain: ["demandActual", "demandForecast", "netGeneration"],
      tickFormat: friendlyTypeName,
      range: ["#ff8ab7", "#6cc5b0", "#a463f2"]
    },
    grid: true,
    marks: [
      Plot.ruleX([currentHour], {strokeOpacity: 0.5}),
      Plot.line(data, {
        x: "date",
        y: (d) => d.value / 1000,
        stroke: "name",
        strokeWidth: 1.2
      }),
      Plot.ruleX(
        compoundTips,
        Plot.pointerX({
          x: "date",
          strokeDasharray: [2, 2],
          channels: {
            date: {value: "date", label: "Time"},
            demandActual: {value: "demandActual", label: friendlyTypeName("demandActual")},
            demandForecast: {value: "demandForecast", label: friendlyTypeName("demandForecast")},
            netGeneration: {value: "netGeneration", label: friendlyTypeName("netGeneration")}
          },
          tip: {
            format: {
              date: (d) => timeFormat("%-d %b %-I %p")(d),
              demandActual: (d) => `${format(".1f")(d / 1000)} GWh`,
              demandForecast: (d) => `${format(".1f")(d / 1000)} GWh`,
              netGeneration: (d) => `${format(".1f")(d / 1000)} GWh`,
              x: false
            },
            fontSize: 12,
            anchor: "bottom",
            frameAnchor: "top"
          }
        })
      )
    ]
  });
}
