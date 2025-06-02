<!-- filepath: /Users/momoamer/Repos/IoT/H4-Device-Location/nuxt-app/components/heatmap.vue -->
<template>
	<div class="w-screen h-screen flex items-center justify-center bg-neutral-100 border-gray-500">
		<canvas ref="canvasRef" width="600" height="400" class="border-gray-500"></canvas>
	</div>
</template>

<script setup lang="ts">
import beacons from "~/shared/beacons";
import { useWebSocket } from "@vueuse/core";

// The canvas ref for our heatmap.
const canvasRef = ref<HTMLCanvasElement | null>(null);
// Devices received from the websocket.
const devices = ref<Device[]>([]);

const { status, data, close, send } = useWebSocket("ws://192.168.0.102:3000/_ws", {
	autoReconnect: {
		retries: 3,
		delay: 1000,
		onFailed() {
			alert("Failed to connect after 3 retries");
		},
	},
});

// Watch reactive data for incoming messages.
watchEffect(() => {
	console.log("WebSocket status:", status.value);
	if (status.value === "OPEN") {
		console.log("WebSocket is open");
	} else if (status.value === "CLOSED") {
		console.log("WebSocket is closed");
	}
	if (data.value) {
		try {
			const device: Device = JSON.parse(data.value);
			devices.value.push(device);
			console.log("Received device data:", device);
		} catch (error) {
			console.error("Error parsing websocket data", error);
		}
	}
});

// Function to render the heatmap (devices and beacons).
function drawHeatmap(ctx: CanvasRenderingContext2D, devices: Device[]) {
	// Clear the canvas.
	ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);

	// Draw devices as green radial gradients.
	devices.forEach((device) => {
		const intensity = 1; // TODO calculate based on signal strength
		const gradient = ctx.createRadialGradient(device.x, device.y, 0, device.x, device.y, 20);
		gradient.addColorStop(0, `rgba(0, 255, 0, ${intensity})`);
		gradient.addColorStop(1, "rgba(0, 255, 0, 0)");
		ctx.fillStyle = gradient;
		ctx.beginPath();
		ctx.arc(device.x, device.y, 40, 0, Math.PI * 2);
		ctx.fill();
	});

	// Draw beacons as blue circles.
	beacons.forEach((beacon: Beacon) => {
		ctx.fillStyle = "blue";
		ctx.beginPath();
		ctx.arc(beacon.x, beacon.y, 10, 0, Math.PI * 2);
		ctx.fill();
		ctx.fillText(beacon.hwid, beacon.x - 18, beacon.y + 20);
	});
}

onMounted(() => {
	const canvas = canvasRef.value;
	if (!canvas) return;
	const ctx = canvas.getContext("2d");
	if (!ctx) return;

	// Redraw the heatmap regularly.
	setInterval(() => {
		drawHeatmap(ctx, devices.value);
	}, 1000);
});
</script>
