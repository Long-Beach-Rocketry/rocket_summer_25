# Plots Kalman filter data, from sim debug output
# Format: Meas_Altitude Meas_Accel | Pred_Altitude Pred_Velo Pred_Accel

from plotly import express as px
import pandas as pd
import sys

if __name__ == "__main__":

    if len(sys.argv) < 3:
        print("Please provide a source then output file name in the first and second command line arguments.")
    else:

        fname = sys.argv[1]

        with open(fname, "r") as r:
            data = [x.strip() for x in r.readlines()]

        mp = []
        ma = []
        pp = []
        pv = []
        pa = []

        for i in data:
            f = [float(x) for x in i.split("|")[0].strip().split(" ")]
            mp.append(f[0])
            ma.append(f[1])

            s = [float(x) for x in i.split("|")[1].strip().split(" ")]
            pp.append(s[0])
            pv.append(s[1])
            pa.append(s[2])


        d = {
            "Time Idx": [float(x) for x in range(0,len(data))],
            "M Alt": mp,
            "M Acc": ma,
            "P Alt": pp,
            "P Vel": pv,
            "P Acc": pa,
        }
        df = pd.DataFrame(d)

        fig = px.line(df, x='Time Idx', y=df.columns[1:])
        fig.write_html(f"{sys.argv[2]}.html")
