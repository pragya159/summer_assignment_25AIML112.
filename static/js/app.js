// BitForge E-Mining Application Logic
const API_BASE = '/api';

const state = {
  user: null,
  token: localStorage.getItem('emining_token') || null,
  plans: [],
  selectedPlan: 'starter',
  selectedDuration: 30,
  calcAmount: 500,
  chartInstance: null,
  currentOrder: null,
  profitTickerValue: 0.000000,
  tickerInterval: null,
  platformStats: null
};

// --- Initialization ---
document.addEventListener('DOMContentLoaded', async () => {
  initEventListeners();
  await loadPlatformStats();
  await loadPlans();
  await checkAuth();
  updateCalculator();
});

// --- Platform Stats ---
async function loadPlatformStats() {
  try {
    const res = await fetch(`${API_BASE}/platform/stats`);
    const data = await res.json();
    if (data.success) {
      state.platformStats = data.stats;
      document.querySelectorAll('.stat-global-hashrate').forEach(el => el.textContent = `${data.stats.global_hashrate_ehs} EH/s`);
      document.querySelectorAll('.stat-total-miners').forEach(el => el.textContent = data.stats.total_miners.toLocaleString());
      document.querySelectorAll('.stat-total-paid').forEach(el => el.textContent = `$${data.stats.total_paid_out_usd.toLocaleString()}`);
      document.querySelectorAll('.stat-platform-fee').forEach(el => el.textContent = `${data.stats.platform_fee_pct}%`);
    }
  } catch (e) {
    console.error('Stats error:', e);
  }
}

// --- Auth System ---
async function checkAuth() {
  if (!state.token) {
    renderLoggedOutUI();
    return;
  }
  try {
    const res = await fetch(`${API_BASE}/auth/me`, {
      headers: { 'Authorization': `Bearer ${state.token}` }
    });
    if (res.ok) {
      const data = await res.json();
      state.user = data.user;
      renderLoggedInUI();
      loadDashboardData();
    } else {
      logout();
    }
  } catch (e) {
    logout();
  }
}

function renderLoggedInUI() {
  document.getElementById('nav-guest-actions').style.display = 'none';
  document.getElementById('nav-user-actions').style.display = 'flex';
  document.getElementById('nav-username-display').textContent = state.user.username;
  document.getElementById('nav-userid-badge').textContent = state.user.user_id;
  document.getElementById('dash-user-name').textContent = state.user.username;
  document.getElementById('dash-user-id').textContent = `User ID: ${state.user.user_id}`;
  document.getElementById('dash-user-avatar').textContent = state.user.username.charAt(0).toUpperCase();
}

function renderLoggedOutUI() {
  document.getElementById('nav-guest-actions').style.display = 'flex';
  document.getElementById('nav-user-actions').style.display = 'none';
  if (state.tickerInterval) clearInterval(state.tickerInterval);
}

async function handleRegister(e) {
  e.preventDefault();
  const username = document.getElementById('reg-username').value.trim();
  const email = document.getElementById('reg-email').value.trim();
  const password = document.getElementById('reg-password').value;
  const alertEl = document.getElementById('reg-alert');

  try {
    const res = await fetch(`${API_BASE}/auth/register`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ username, email, password })
    });
    const data = await res.json();
    if (data.success) {
      state.token = data.token;
      state.user = data.user;
      localStorage.setItem('emining_token', data.token);
      closeAllModals();
      renderLoggedInUI();
      loadDashboardData();
      showToast('Account created successfully! Welcome aboard.', 'success');
    } else {
      alertEl.textContent = data.error;
      alertEl.style.display = 'block';
    }
  } catch (err) {
    alertEl.textContent = 'Server error during registration. Please retry.';
    alertEl.style.display = 'block';
  }
}

async function handleLogin(e) {
  e.preventDefault();
  const login = document.getElementById('login-identifier').value.trim();
  const password = document.getElementById('login-password').value;
  const alertEl = document.getElementById('login-alert');

  try {
    const res = await fetch(`${API_BASE}/auth/login`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ login, password })
    });
    const data = await res.json();
    if (data.success) {
      state.token = data.token;
      state.user = data.user;
      localStorage.setItem('emining_token', data.token);
      closeAllModals();
      renderLoggedInUI();
      loadDashboardData();
      showToast('Logged in successfully!', 'success');
    } else {
      alertEl.textContent = data.error;
      alertEl.style.display = 'block';
    }
  } catch (err) {
    alertEl.textContent = 'Failed to connect. Please check credentials.';
    alertEl.style.display = 'block';
  }
}

async function logout() {
  try {
    await fetch(`${API_BASE}/auth/logout`, {
      method: 'POST',
      headers: state.token ? { 'Authorization': `Bearer ${state.token}` } : {}
    });
  } catch (e) {}
  state.token = null;
  state.user = null;
  localStorage.removeItem('emining_token');
  renderLoggedOutUI();
  switchView('home');
  showToast('Logged out safely.', 'info');
}

// --- Mining Plans & ROI Calculator ---
async function loadPlans() {
  try {
    const res = await fetch(`${API_BASE}/plans`);
    const data = await res.json();
    if (data.success) {
      state.plans = data.plans;
      renderPlansGrid(data.plans);
    }
  } catch (e) {
    console.error('Failed loading plans:', e);
  }
}

function renderPlansGrid(plans) {
  const container = document.getElementById('plans-cards-grid');
  if (!container) return;

  container.innerHTML = plans.map(p => {
    const isFeatured = p.plan_code === 'enterprise';
    return `
      <div class="plan-card ${isFeatured ? 'featured' : ''}">
        ${isFeatured ? '<span class="badge-pill badge-green plan-badge">Most Popular</span>' : ''}
        <h3 class="plan-title">${p.name}</h3>
        <p style="font-size: 0.84rem; color: var(--text-muted);">${p.description}</p>
        <div class="plan-roi">
          +${p.daily_roi_pct}% <span>/ day</span>
        </div>
        <ul class="plan-specs">
          <li class="plan-spec-item">
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M13 2L3 14h9l-1 8 10-12h-9l1-8z"/></svg>
            <strong>${p.duration_days} Days</strong> Duration
          </li>
          <li class="plan-spec-item">
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="2" y="2" width="20" height="8" rx="2"/><rect x="2" y="14" width="20" height="8" rx="2"/></svg>
            Hardware: ${p.hardware}
          </li>
          <li class="plan-spec-item">
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><path d="M12 6v6l4 2"/></svg>
            Instant Daily Payouts
          </li>
          <li class="plan-spec-item">
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 2v20M17 5H9.5a3.5 3.5 0 0 0 0 7h5a3.5 3.5 0 0 1 0 7H6"/></svg>
            Min: $${p.min_investment.toLocaleString()} — Max: $${p.max_investment.toLocaleString()}
          </li>
          <li class="plan-spec-item" style="color: var(--accent-gold);">
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 22s8-4 8-10V5l-8-3-8 3v7c0 6 8 10 8 10z"/></svg>
            Transparent 2% Profit Fee
          </li>
        </ul>
        <button class="btn btn-primary" onclick="openCheckoutForPlan('${p.plan_code}', ${p.min_investment})">
          Select ${p.name}
        </button>
      </div>
    `;
  }).join('');
}

async function updateCalculator() {
  const amount = parseFloat(state.calcAmount);
  const planCode = state.selectedPlan;
  const duration = parseInt(state.selectedDuration);

  try {
    const res = await fetch(`${API_BASE}/calculator/calculate`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ amount, plan_code: planCode, duration })
    });
    const result = await res.json();
    if (result.success) {
      const data = result.data;
      
      // Update UI numbers
      document.getElementById('calc-daily-profit').textContent = `+$${data.daily.net.toFixed(2)}`;
      document.getElementById('calc-weekly-profit').textContent = `+$${data.weekly.net.toFixed(2)}`;
      document.getElementById('calc-monthly-profit').textContent = `+$${data.monthly.net.toFixed(2)}`;
      document.getElementById('calc-total-return').textContent = `$${data.total.total_return.toFixed(2)}`;
      document.getElementById('calc-roi-badge').textContent = `+${data.total.net_roi_pct.toFixed(1)}% NET ROI`;
      document.getElementById('calc-hashrate-val').textContent = `${data.hashrate_ths} TH/s`;

      // Fee Breakdown
      document.getElementById('fee-gross-profit').textContent = `$${data.total.gross_profit.toFixed(2)}`;
      document.getElementById('fee-platform-cut').textContent = `-$${data.total.platform_fee.toFixed(2)} (2%)`;
      document.getElementById('fee-net-profit').textContent = `$${data.total.net_profit.toFixed(2)}`;

      // Render Chart
      renderRoiChart(data.chart);
    }
  } catch (e) {
    console.error('Calculation error:', e);
  }
}

function renderRoiChart(chartData) {
  const ctx = document.getElementById('roiChart');
  if (!ctx || typeof Chart === 'undefined') return;

  if (state.chartInstance) {
    state.chartInstance.destroy();
  }

  state.chartInstance = new Chart(ctx, {
    type: 'line',
    data: {
      labels: chartData.labels,
      datasets: [
        {
          label: 'Net User Return (USD)',
          data: chartData.net,
          borderColor: '#16a34a',
          backgroundColor: 'rgba(22, 163, 74, 0.08)',
          borderWidth: 2.5,
          tension: 0.35,
          fill: true,
          pointRadius: 3,
          pointHoverRadius: 6
        },
        {
          label: 'Initial Principal',
          data: chartData.principal,
          borderColor: '#94a3b8',
          borderDash: [5, 5],
          borderWidth: 1.5,
          fill: false,
          pointRadius: 0
        }
      ]
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: {
        legend: {
          display: true,
          labels: { color: '#475569', font: { size: 11, weight: '600' } }
        },
        tooltip: {
          backgroundColor: '#0f172a',
          titleColor: '#fff',
          bodyColor: '#4ade80',
          borderColor: '#e2e8f0',
          borderWidth: 1
        }
      },
      scales: {
        x: {
          grid: { color: 'rgba(0, 0, 0, 0.05)' },
          ticks: { color: '#64748b', font: { size: 10 } }
        },
        y: {
          grid: { color: 'rgba(0, 0, 0, 0.05)' },
          ticks: { color: '#64748b', font: { size: 10 } }
        }
      }
    }
  });
}

// --- Dashboard & Profit Tracking ---
async function loadDashboardData() {
  if (!state.token) return;
  try {
    const [walletRes, contractsRes, txRes] = await Promise.all([
      fetch(`${API_BASE}/wallet/summary`, { headers: { 'Authorization': `Bearer ${state.token}` } }),
      fetch(`${API_BASE}/contracts/active`, { headers: { 'Authorization': `Bearer ${state.token}` } }),
      fetch(`${API_BASE}/payments/history`, { headers: { 'Authorization': `Bearer ${state.token}` } })
    ]);

    if (walletRes.ok) {
      const wData = await walletRes.json();
      const w = wData.wallet;
      document.getElementById('dash-balance-usdt').textContent = `${w.usdt_balance.toFixed(2)} USDT`;
      document.getElementById('dash-balance-usd').textContent = `$${w.usd_balance.toFixed(2)}`;
      document.getElementById('dash-total-balance').textContent = `$${w.total_balance_usd.toFixed(2)}`;
      document.getElementById('dash-active-hashrate').textContent = `${w.active_hashrate_ths} TH/s`;
      document.getElementById('dash-total-profit').textContent = `+$${w.total_profit.toFixed(4)}`;
      document.getElementById('dash-platform-fee-paid').textContent = `$${w.platform_fee_paid_2pct.toFixed(4)}`;
      
      // Start live animated profit ticker
      startProfitTicker(w.total_profit, w.active_hashrate_ths);
    }

    if (contractsRes.ok) {
      const cData = await contractsRes.json();
      renderContractsTable(cData.contracts);
    }

    if (txRes.ok) {
      const txData = await txRes.json();
      renderTransactionsTable(txData.transactions);
    }
  } catch (e) {
    console.error('Error loading dashboard:', e);
  }
}

function startProfitTicker(currentProfit, activeHashrate) {
  if (state.tickerInterval) clearInterval(state.tickerInterval);
  state.profitTickerValue = currentProfit;

  // If user has active hashrate, animate satoshis accumulating every 2 seconds
  state.tickerInterval = setInterval(async () => {
    if (activeHashrate > 0) {
      const microYield = (activeHashrate * 0.00000035);
      state.profitTickerValue += microYield;
      const el = document.getElementById('live-profit-counter');
      if (el) {
        el.textContent = `+$${state.profitTickerValue.toFixed(6)} USDT`;
      }
    }
  }, 2000);
}

function renderContractsTable(contracts) {
  const container = document.getElementById('contracts-table-body');
  if (!container) return;

  if (contracts.length === 0) {
    container.innerHTML = `<tr><td colspan="6" style="text-align: center; color: var(--text-muted); padding: 30px;">No active mining contracts yet. Choose a plan or use the ROI calculator to deploy hash power.</td></tr>`;
    return;
  }

  container.innerHTML = contracts.map(c => `
    <tr>
      <td><strong>${c.contract_id}</strong></td>
      <td>${c.plan_name}</td>
      <td><span class="badge-pill badge-cyan">${c.hashrate_ths} TH/s</span></td>
      <td>$${c.amount.toFixed(2)} ${c.currency}</td>
      <td style="color: var(--accent-green);">+$${c.daily_profit.toFixed(4)} / day</td>
      <td>
        <span class="status-pill status-${c.status === 'ACTIVE' ? 'credited' : 'pending'}">${c.status}</span>
      </td>
    </tr>
  `).join('');
}

function renderTransactionsTable(txs) {
  const container = document.getElementById('transactions-table-body');
  if (!container) return;

  if (txs.length === 0) {
    container.innerHTML = `<tr><td colspan="6" style="text-align: center; color: var(--text-muted); padding: 30px;">No transaction records found.</td></tr>`;
    return;
  }

  container.innerHTML = txs.map(tx => {
    let statusClass = 'pending';
    if (tx.status === 'CREDITED') statusClass = 'credited';
    else if (tx.status === 'FAILED') statusClass = 'failed';

    return `
      <tr>
        <td><code>${tx.tx_hash}</code></td>
        <td><strong>${tx.type}</strong></td>
        <td>${tx.amount.toFixed(2)} ${tx.currency}</td>
        <td>${tx.payment_method.replace('_', ' ')}</td>
        <td><span class="status-pill status-${statusClass}">${tx.status}</span></td>
        <td style="font-size: 0.8rem; color: var(--text-dim);">${tx.created_at}</td>
      </tr>
    `;
  }).join('');
}

// --- Payment & "Pay on Website" Checkout Flow ---
function openCheckoutForPlan(planCode, defaultAmount) {
  if (!state.token) {
    openModal('login-modal');
    showToast('Please login or register to invest in a mining plan.', 'info');
    return;
  }

  document.getElementById('checkout-plan-code').value = planCode;
  document.getElementById('checkout-amount').value = defaultAmount || 100;
  
  // Set default method
  selectPaymentMethod('USDT_TRC20');
  
  // Hide verification box initially
  document.getElementById('checkout-verify-panel').style.display = 'none';
  document.getElementById('checkout-initiate-btn').style.display = 'block';

  openModal('checkout-modal');
}

function selectPaymentMethod(method) {
  document.querySelectorAll('.pay-method-btn').forEach(btn => btn.classList.remove('active'));
  const target = document.querySelector(`.pay-method-btn[data-method="${method}"]`);
  if (target) target.classList.add('active');

  const cardDetails = document.getElementById('checkout-card-fields');
  const cryptoDetails = document.getElementById('checkout-crypto-fields');

  if (method === 'USD_CARD') {
    cardDetails.style.display = 'block';
    cryptoDetails.style.display = 'none';
  } else {
    cardDetails.style.display = 'none';
    cryptoDetails.style.display = 'block';
  }
}

async function initiateCheckoutOrder() {
  const amount = parseFloat(document.getElementById('checkout-amount').value);
  const planCode = document.getElementById('checkout-plan-code').value;
  const activeMethodBtn = document.querySelector('.pay-method-btn.active');
  const paymentMethod = activeMethodBtn ? activeMethodBtn.dataset.method : 'USDT_TRC20';
  const currency = paymentMethod.startsWith('USDT') ? 'USDT' : 'USD';

  if (!amount || amount <= 0) {
    showToast('Please specify a valid investment amount.', 'error');
    return;
  }

  try {
    const res = await fetch(`${API_BASE}/payments/checkout`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${state.token}`
      },
      body: JSON.stringify({
        amount,
        currency,
        payment_method: paymentMethod,
        plan_code: planCode
      })
    });

    const data = await res.json();
    if (data.success) {
      state.currentOrder = data.order;
      displayOrderVerification(data.order);
    } else {
      showToast(data.error, 'error');
    }
  } catch (err) {
    showToast('Could not initiate payment order.', 'error');
  }
}

function displayOrderVerification(order) {
  document.getElementById('checkout-initiate-btn').style.display = 'none';
  const panel = document.getElementById('checkout-verify-panel');
  panel.style.display = 'block';

  document.getElementById('verify-order-id').textContent = order.tx_hash;
  document.getElementById('verify-order-amount').textContent = `${order.amount.toFixed(2)} ${order.currency}`;
  document.getElementById('verify-order-network').textContent = order.network;
  
  const statusBadge = document.getElementById('verify-status-badge');
  statusBadge.className = 'status-pill status-pending';
  statusBadge.textContent = 'PENDING VERIFICATION';

  if (order.payment_method.startsWith('USDT')) {
    document.getElementById('verify-address-wrap').style.display = 'block';
    document.getElementById('verify-pay-address').value = order.pay_address;
    
    // Dynamic QR image
    const qrImg = document.getElementById('verify-qr-img');
    qrImg.src = `https://api.qrserver.com/v1/create-qr-code/?size=160x160&data=${encodeURIComponent(order.pay_address)}`;
  } else {
    document.getElementById('verify-address-wrap').style.display = 'none';
  }
}

async function verifyCurrentPayment(simulateFail = false) {
  if (!state.currentOrder) return;
  const statusBadge = document.getElementById('verify-status-badge');
  const verifyBtn = document.getElementById('btn-execute-verify');
  
  verifyBtn.disabled = true;
  statusBadge.className = 'status-pill status-pending';
  statusBadge.innerHTML = '<span class="pulse-dot"></span> Verifying Gateway API...';

  // Simulate network broadcast step
  setTimeout(async () => {
    try {
      const res = await fetch(`${API_BASE}/payments/verify`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${state.token}`
        },
        body: JSON.stringify({
          tx_hash: state.currentOrder.tx_hash,
          simulate_fail: simulateFail
        })
      });

      const data = await res.json();
      verifyBtn.disabled = false;

      if (data.success && data.result.status === 'CREDITED') {
        statusBadge.className = 'status-pill status-credited';
        statusBadge.textContent = 'CREDITED ✅';
        showToast('Payment verified! Hash power allocated to your wallet.', 'success');
        await loadDashboardData();
        setTimeout(() => {
          closeAllModals();
          switchView('dashboard');
        }, 1800);
      } else {
        statusBadge.className = 'status-pill status-failed';
        statusBadge.textContent = 'FAILED ❌';
        showToast(data.result?.message || 'Verification failed. Please retry.', 'error');
        await loadDashboardData();
      }
    } catch (err) {
      verifyBtn.disabled = false;
      statusBadge.className = 'status-pill status-failed';
      statusBadge.textContent = 'FAILED ❌';
      showToast('Gateway connection timed out.', 'error');
    }
  }, 1400);
}

// --- Withdrawal System ---
async function handleWithdrawal(e) {
  e.preventDefault();
  const amount = parseFloat(document.getElementById('withdraw-amount').value);
  const currency = document.getElementById('withdraw-currency').value;
  const address = document.getElementById('withdraw-address').value.trim();
  const alertEl = document.getElementById('withdraw-alert');

  try {
    const res = await fetch(`${API_BASE}/payouts/request`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${state.token}`
      },
      body: JSON.stringify({ amount, currency, address })
    });

    const data = await res.json();
    if (data.success) {
      closeAllModals();
      showToast(data.payout.message, 'success');
      loadDashboardData();
    } else {
      alertEl.textContent = data.error;
      alertEl.style.display = 'block';
    }
  } catch (err) {
    alertEl.textContent = 'Withdrawal request failed. Please check network.';
    alertEl.style.display = 'block';
  }
}

// --- Claim Accrued Profits ---
async function claimAccruedYields() {
  if (!state.token) return;
  try {
    const res = await fetch(`${API_BASE}/profits/accrue`, {
      method: 'POST',
      headers: { 'Authorization': `Bearer ${state.token}` }
    });
    const data = await res.json();
    if (data.success) {
      const net = data.accrual.net_profit;
      const fee = data.accrual.platform_fee;
      if (net > 0) {
        showToast(`Claimed +$${net.toFixed(4)} USDT! (2% platform fee: $${fee.toFixed(4)})`, 'success');
      } else {
        showToast('Yields are compounding actively! Real-time satoshis running.', 'info');
      }
      loadDashboardData();
    }
  } catch (e) {
    showToast('Failed to claim yields.', 'error');
  }
}

// --- Support & Chatbot ---
async function sendChatMessage(presetText = null) {
  const input = document.getElementById('chat-input-text');
  const text = presetText || input.value.trim();
  if (!text) return;

  const messagesContainer = document.getElementById('chat-messages-container');

  // Append user message
  const userMsg = document.createElement('div');
  userMsg.className = 'chat-msg user';
  userMsg.textContent = text;
  messagesContainer.appendChild(userMsg);
  input.value = '';
  messagesContainer.scrollTop = messagesContainer.scrollHeight;

  // Bot typing indicator
  const typingMsg = document.createElement('div');
  typingMsg.className = 'chat-msg bot';
  typingMsg.innerHTML = '<em>Thinking...</em>';
  messagesContainer.appendChild(typingMsg);
  messagesContainer.scrollTop = messagesContainer.scrollHeight;

  try {
    const res = await fetch(`${API_BASE}/chatbot/message`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ message: text })
    });
    const data = await res.json();
    typingMsg.innerHTML = formatMarkdownToHtml(data.reply);

    // Update suggestions
    const suggContainer = document.getElementById('chat-suggestions-container');
    if (data.suggestions && data.suggestions.length > 0) {
      suggContainer.innerHTML = data.suggestions.map(s => 
        `<span class="suggestion-chip" onclick="sendChatMessage('${s.replace(/'/g, "\\'")}')">${s}</span>`
      ).join('');
    }
    messagesContainer.scrollTop = messagesContainer.scrollHeight;
  } catch (e) {
    typingMsg.textContent = 'Support assistant offline temporarily. Please check FAQs.';
  }
}

function formatMarkdownToHtml(text) {
  return text
    .replace(/\*\*(.*?)\*\*/g, '<strong>$1</strong>')
    .replace(/\n\n/g, '<br><br>')
    .replace(/\n/g, '<br>')
    .replace(/• /g, '&bull; ');
}

// --- Support Ticket Submission ---
async function handleTicketSubmit(e) {
  e.preventDefault();
  if (!state.token) {
    openModal('login-modal');
    showToast('Please login to create a support ticket.', 'info');
    return;
  }

  const subject = document.getElementById('ticket-subject').value.trim();
  const category = document.getElementById('ticket-category').value;
  const message = document.getElementById('ticket-message').value.trim();
  const alertEl = document.getElementById('ticket-alert');

  try {
    const res = await fetch(`${API_BASE}/support/tickets`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${state.token}`
      },
      body: JSON.stringify({ subject, category, message })
    });
    const data = await res.json();
    if (data.success) {
      alertEl.textContent = `Ticket ${data.ticket_id} created! Response will arrive in your email.`;
      alertEl.style.color = 'var(--accent-green)';
      alertEl.style.display = 'block';
      document.getElementById('ticket-subject').value = '';
      document.getElementById('ticket-message').value = '';
    } else {
      alertEl.textContent = data.error;
      alertEl.style.color = 'var(--accent-red)';
      alertEl.style.display = 'block';
    }
  } catch (e) {
    alertEl.textContent = 'Ticket submission failed.';
    alertEl.style.display = 'block';
  }
}

// --- UI Helpers & Modals ---
function openModal(id) {
  closeAllModals();
  const modal = document.getElementById(id);
  if (modal) modal.classList.add('active');
}

function closeAllModals() {
  document.querySelectorAll('.modal-overlay').forEach(m => m.classList.remove('active'));
}

function switchView(viewName) {
  const homeView = document.getElementById('view-home');
  const dashView = document.getElementById('view-dashboard');

  if (viewName === 'dashboard') {
    if (!state.token) {
      openModal('login-modal');
      return;
    }
    homeView.style.display = 'none';
    dashView.classList.add('active');
    loadDashboardData();
    window.scrollTo({ top: 0, behavior: 'smooth' });
  } else {
    homeView.style.display = 'block';
    dashView.classList.remove('active');
    window.scrollTo({ top: 0, behavior: 'smooth' });
  }
}

function showToast(message, type = 'info') {
  const toast = document.createElement('div');
  toast.style.position = 'fixed';
  toast.style.bottom = '24px';
  toast.style.left = '50%';
  toast.style.transform = 'translateX(-50%)';
  toast.style.background = type === 'success' ? '#1c2824' : (type === 'error' ? '#2a1b1b' : '#202228');
  toast.style.color = type === 'success' ? '#4ade80' : (type === 'error' ? '#f87171' : '#f4f4f5');
  toast.style.border = '1px solid #313540';
  toast.style.padding = '12px 24px';
  toast.style.borderRadius = '30px';
  toast.style.fontWeight = '600';
  toast.style.fontSize = '0.9rem';
  toast.style.boxShadow = '0 10px 25px rgba(0,0,0,0.5)';
  toast.style.zIndex = '9999';
  toast.style.transition = 'all 0.3s ease';
  toast.textContent = message;
  document.body.appendChild(toast);

  setTimeout(() => {
    toast.style.opacity = '0';
    setTimeout(() => toast.remove(), 300);
  }, 3200);
}

function copyAddressToClipboard() {
  const input = document.getElementById('verify-pay-address');
  input.select();
  navigator.clipboard.writeText(input.value);
  showToast('Wallet address copied to clipboard!', 'success');
}

// --- Event Listeners Setup ---
function initEventListeners() {
  // Navigation Links
  document.querySelectorAll('.nav-link[data-view]').forEach(link => {
    link.addEventListener('click', (e) => {
      e.preventDefault();
      const view = link.dataset.view;
      switchView(view);
    });
  });

  // Auth Modals
  document.getElementById('btn-nav-login')?.addEventListener('click', () => openModal('login-modal'));
  document.getElementById('btn-nav-register')?.addEventListener('click', () => openModal('register-modal'));
  document.getElementById('btn-nav-logout')?.addEventListener('click', logout);
  document.getElementById('btn-nav-dashboard')?.addEventListener('click', () => switchView('dashboard'));
  document.getElementById('link-switch-to-reg')?.addEventListener('click', () => openModal('register-modal'));
  document.getElementById('link-switch-to-login')?.addEventListener('click', () => openModal('login-modal'));

  // Forms
  document.getElementById('form-register')?.addEventListener('submit', handleRegister);
  document.getElementById('form-login')?.addEventListener('submit', handleLogin);
  document.getElementById('form-withdraw')?.addEventListener('submit', handleWithdrawal);
  document.getElementById('form-support-ticket')?.addEventListener('submit', handleTicketSubmit);

  // Close modals
  document.querySelectorAll('.modal-close').forEach(btn => {
    btn.addEventListener('click', closeAllModals);
  });
  document.querySelectorAll('.modal-overlay').forEach(overlay => {
    overlay.addEventListener('click', (e) => {
      if (e.target === overlay) closeAllModals();
    });
  });

  // Calculator Sliders & Inputs
  const calcSlider = document.getElementById('calc-slider');
  const calcNumberInput = document.getElementById('calc-number-input');

  if (calcSlider && calcNumberInput) {
    calcSlider.addEventListener('input', (e) => {
      calcNumberInput.value = e.target.value;
      state.calcAmount = e.target.value;
      updateCalculator();
    });
    calcNumberInput.addEventListener('input', (e) => {
      calcSlider.value = e.target.value;
      state.calcAmount = e.target.value;
      updateCalculator();
    });
  }

  // Calculator Presets
  document.querySelectorAll('.btn-preset').forEach(btn => {
    btn.addEventListener('click', () => {
      document.querySelectorAll('.btn-preset').forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
      const val = btn.dataset.amount;
      calcSlider.value = val;
      calcNumberInput.value = val;
      state.calcAmount = val;
      updateCalculator();
    });
  });

  // Calculator Duration Pills
  document.querySelectorAll('.duration-pill').forEach(pill => {
    pill.addEventListener('click', () => {
      document.querySelectorAll('.duration-pill').forEach(p => p.classList.remove('active'));
      pill.classList.add('active');
      state.selectedDuration = pill.dataset.days;
      updateCalculator();
    });
  });

  // Calculator CTA
  document.getElementById('btn-calc-invest')?.addEventListener('click', () => {
    openCheckoutForPlan(state.selectedPlan, state.calcAmount);
  });

  // Checkout Payment Method Selector
  document.querySelectorAll('.pay-method-btn').forEach(btn => {
    btn.addEventListener('click', () => selectPaymentMethod(btn.dataset.method));
  });

  // Checkout Initiate & Verify
  document.getElementById('checkout-initiate-btn')?.addEventListener('click', initiateCheckoutOrder);
  document.getElementById('btn-execute-verify')?.addEventListener('click', () => verifyCurrentPayment(false));
  document.getElementById('btn-simulate-fail')?.addEventListener('click', () => verifyCurrentPayment(true));

  // Dashboard Actions
  document.getElementById('btn-open-deposit-modal')?.addEventListener('click', () => openCheckoutForPlan('starter', 500));
  document.getElementById('btn-open-withdraw-modal')?.addEventListener('click', () => openModal('withdraw-modal'));
  document.getElementById('btn-claim-yields')?.addEventListener('click', claimAccruedYields);

  // Chatbot FAB & Window
  const chatbotFab = document.getElementById('chatbot-fab-btn');
  const chatbotPanel = document.getElementById('chatbot-panel');
  const chatbotClose = document.getElementById('chatbot-close-btn');

  chatbotFab?.addEventListener('click', () => chatbotPanel.classList.toggle('open'));
  chatbotClose?.addEventListener('click', () => chatbotPanel.classList.remove('open'));
  document.getElementById('chat-send-btn')?.addEventListener('click', () => sendChatMessage());
  document.getElementById('chat-input-text')?.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') sendChatMessage();
  });

  // FAQ Accordions
  document.querySelectorAll('.faq-item').forEach(item => {
    item.querySelector('.faq-question').addEventListener('click', () => {
      item.classList.toggle('open');
    });
  });
}
