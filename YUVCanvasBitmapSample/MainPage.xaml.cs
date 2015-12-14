using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

using Win2D_YUVCanvasBitmap;
using Windows.UI;
using Microsoft.Graphics.Canvas;
using Microsoft.Graphics.Canvas.UI.Xaml;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace YUVCanvasBitmapSample
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        byte[] source;

        CanvasDevice device;

        CanvasRenderTarget target;

        CanvasSwapChain swapChain;

        public MainPage()
        {
            this.InitializeComponent();

            source = File.ReadAllBytes("output.yuv");
        }

        private void SwapChainPanel_Loaded(object sender, RoutedEventArgs e)
        {
            var swapChainPanel = sender as CanvasSwapChainPanel;

            this.device = CanvasDevice.GetSharedDevice();

            this.swapChain = new CanvasSwapChain(this.device, (int)swapChainPanel.ActualWidth, (int)swapChainPanel.ActualHeight, 96);

            swapChainPanel.SwapChain = this.swapChain;

            this.target = new CanvasRenderTarget(this.device, (int)swapChainPanel.ActualWidth, (int)swapChainPanel.ActualHeight, 96);

            int width = 4000;

            int height = 3000;

            byte[] dataY = this.source.Skip(0).Take(width * height).ToArray();

            byte[] dataU = this.source.Skip(dataY.Length).Take((int)((width / 2) * (height / 2))).ToArray();

            byte[] dataV = this.source.Skip(dataY.Length + dataU.Length).Take(((width / 2) * (height / 2))).ToArray();

            YUVCanvasBitmap.render(this.target, dataY, dataU, dataV);

            this.draw();
        }

        private void SwapChainPanel_Unloaded(object sender, RoutedEventArgs e)
        {

        }

        private void SwapChainPanel_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (swapChain == null) { return; }

            this.swapChain.ResizeBuffers(e.NewSize);

            this.draw();
        }

        private void SwapChainPanel_CompositionScaleChanged(SwapChainPanel sender, object args)
        {

        }

        private void draw()
        {
            using (var session = this.swapChain.CreateDrawingSession(Colors.Black))
            {
                if (this.target != null)
                {
                    session.DrawImage(this.target, new Rect(0, 0, this.swapChain.Size.Width, this.swapChain.Size.Height), new Rect(0, 0, 4000, 3000));
                }
            }

            this.swapChain.Present();
        }
    }
}
