<?php

/**
 * @file
 * Admin callbacks for Islandora Preingest Transform
 */

/**
 * Admin form.
 */
function delft_admin($form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $form = array();
  $form['#tree'] = TRUE;
  $password = variable_get('delftdora_fedora_password', '');
  $executable_path = variable_get('delftdora_fedora_path', '/usr/local/fedora/client/bin');
  $log_path = variable_get('delftdora_fedora_log_path', '/usr/local/fedora/server/logs');

  $form['delftdora_fedora_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Password'),
    '#description' => t('Password for user fedoraAdmin.'),
    '#default_value' => $password,
  );

  $form['delftdora_fedora_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Executable Path'),
    '#description' => t('Full server Path to fedora-modify script'),
    '#default_value' => $executable_path,
  );
  $form['delftdora_fedora_log_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Logging Path'),
    '#description' => t('Full server path to Fedora logging directory'),
    '#default_value' => $log_path,
  );

  $form['rels_ext_select'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Models to Replace RELS-EXT'),
    '#description' => t('Objects of selected content models will have RELS-EXT replaced by the output of sdef:makeDS'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['rels_ext_select']['cmodels'] = islandora_content_model_select_table_form_element('delftdora_rels_content_models');

  $form['batch_modify'] = array(
    '#type' => 'fieldset',
    '#title' => t('Batch Modify Configuration'),
    '#description' => t('Per content model Batch Modify configuration.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (empty($form_state['values']['batch_modify'])) {
    $batch_modify = _delftdora_normalize_batch_modify_cmodels(variable_get('delft_batch_modify_cmodels', array()));
  }
  else {
    $batch_modify = _delftdora_normalize_batch_modify_cmodels($form_state['values']['batch_modify']);
  }

  if (empty($batch_modify)) {
    $form_state['bm_count'] = 1;
  }
  else {
    $form_state['bm_count'] = count($batch_modify) + 1;
  }

  for ($i = 1; $i <= $form_state['bm_count']; $i++) {
    $form['batch_modify'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('CModel/Batch Modify Config @num', array('@num' => $i)),
    );
    $form['batch_modify'][$i]['cmodel'] = array(
      '#type' => 'textfield',
      '#title' => t('CModel'),
    );
    if (isset($batch_modify[$i]['cmodel'])) {
      $form['batch_modify'][$i]['cmodel']['#default_value'] = $batch_modify[$i]['cmodel'];
    }
    $form['batch_modify'][$i]['sdef'] = array(
      '#type' => 'textfield',
      '#title' => t('SDef'),
    );
    if (isset($batch_modify[$i]['sdef'])) {
      $form['batch_modify'][$i]['sdef']['#default_value'] = $batch_modify[$i]['sdef'];
    }
    $form['batch_modify'][$i]['method'] = array(
      '#type' => 'textfield',
      '#title' => t('Method'),
    );
    if (isset($batch_modify[$i]['method'])) {
      $form['batch_modify'][$i]['method']['#default_value'] = $batch_modify[$i]['method'];
    }
  }
  $form['batch_modify']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#submit' => array('delftdora_admin_batch_modify'),
  );

  $form['islandora_transform_submit'] = array(
    '#value' => t('Save Configuration'),
    '#type' => 'submit',
  );
  return $form;
}

/**
 * Admin form submit.
 */
function delft_admin_submit($form, &$form_state) {
  $rels_selected_cmodels = array_filter($form_state['values']['rels_ext_select']['cmodels']);

  variable_set('delftdora_fedora_password', $form_state['values']['delftdora_fedora_password']);
  variable_set('delftdora_fedora_path', $form_state['values']['delftdora_fedora_path']);
  variable_set('delftdora_fedora_log_path', $form_state['values']['delftdora_fedora_log_path']);
  variable_set('delftdora_rels_content_models', $rels_selected_cmodels);

  $bm_values = _delftdora_normalize_batch_modify_cmodels($form_state['values']['batch_modify']);
  variable_set('delft_batch_modify_cmodels', $bm_values);

  drupal_set_message(t('Settings saved'));
}

/**
 * Submit callback to add batch modify configuration.
 */
function delftdora_admin_batch_modify($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Clean up the $form_state['values']['batch_modify'] tree.
 */
function _delftdora_normalize_batch_modify_cmodels($values) {
  $bm_values = array();
  foreach ($values as $k => $v) {
    $values = (is_array($v)) ? implode('', array_values($v)) : '';
    if (is_numeric($k) && !empty($values)) {
      $bm_values[$k] = $v;
    }
  }
  return $bm_values;
}
